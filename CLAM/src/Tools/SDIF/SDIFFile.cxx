#include "SDIFFile.hxx"

using namespace CLAM;

namespace SDIF
{

	File::File(const char* filename,Mode mode)
	:DataFileIO(filename,mode)
	{
		mSkipData = false;
		mFirstAccess = true;
	}

	File::~File()
	{
	}

	void File::Read(Storage& storage)
	{
		while (!Done()) {
			Frame* frame = new Frame;
			Read(*frame);
			storage.Add(frame);
		}
	}

	void File::Write(const Storage& storage)
	{
		WriteInit();		
		typedef std::list<Frame*>::const_iterator iterator;
		
		iterator it = storage.Begin();
		iterator end = storage.End();
		
		while (it!=end)
		{
			Frame* frame = *it;
			Write(*frame);
			it++;
		}
	}

	void File::Read(Type& type)
	{
		DataFileIO::Read(type.mData,4);
	}

	void File::Write(const Type& type)
	{
		DataFileIO::Write(type.mData,4);
	}

	void File::Read(FrameHeader& header)
	{
		Read(header.mType);
		Read(header.mSize);
	}

	void File::Write(const FrameHeader& header)
	{
		Write(header.mType);
		Write(header.mSize);
	}

	void File::Read(DataFrameHeader& header)
	{
		Read((FrameHeader&)header);
		Read(header.mTime);
		Read(header.mStreamId);
		Read(header.mnMatrices);
	}

	void File::Write(const DataFrameHeader& header)
	{
		Write((FrameHeader&)header);
		Write(header.mTime);
		Write(header.mStreamId);
		Write(header.mnMatrices);
	}

	void File::Read(Frame& frame)
	{
		if (mFirstAccess)
		{
			mFirstAccess = false;
			OpeningsFrame opening;
			Read(opening);
		}

		Read(frame.mHeader);
		int tmp = frame.mHeader.mnMatrices;
		frame.mHeader.mnMatrices = 0; // frame.Add will increase this
		for (int i=0;i<tmp;i++)
		{
			Matrix* pMatrix = new Matrix;
			File::Read(*pMatrix);
			frame.Add(pMatrix);
		}
	}

	void File::Write(const Frame& frame)
	{
		if (mFirstAccess)
		{
			mFirstAccess = false;
			OpeningsFrame opening;
			Write(opening);
		}

		Write(frame.mHeader);
		
		typedef std::list<Matrix*>::const_iterator iterator;

		iterator it = frame.mMatrixList.begin();
		iterator end = frame.mMatrixList.end();

		while (it!=end)
		{
			Matrix* pMatrix = *it;
			File::Write(*pMatrix);
			it++;
		}
	}

	void File::Read(OpeningsFrame& frame)
	{
		Read(frame.mHeader);
		Read(frame.mSpecVersion);
		Read(frame.mStandardTypesVersion);
	}

	void File::Write(const OpeningsFrame& frame)
	{
		Write(frame.mHeader);
		Write(frame.mSpecVersion);
		Write(frame.mStandardTypesVersion);
	}

	void File::Read(MatrixHeader& header)
	{
		Read(header.mType);
		TInt32 tmp;
		Read(tmp);
		header.mDataType = (DataType) tmp;
		Read(header.mnRows);
		Read(header.mnColumns);
	}

	void File::Write(const MatrixHeader& header)
	{ 
		Write(header.mType);
		TInt32 tmp = header.mDataType;
		Write(tmp);
		Write(header.mnRows);
		Write(header.mnColumns);
	}

	void File::Read(Matrix& matrix)
	{
		Read(matrix.mHeader);
		if (mSkipData)
		{
			SkipMatrixData(matrix);
		} else {
			ReadMatrixData(matrix);
		}
	}

	void File::Write(const Matrix& matrix)
	{
		Write(matrix.mHeader);
		WriteMatrixData(matrix);
	}

	void File::SkipMatrixData(const Matrix& matrix)
	{
		TUInt32 size = matrix.mHeader.mnColumns*matrix.mHeader.mnRows*
			((matrix.mHeader.mDataType)&0xFF);
		TUInt32 padding = 8-size&7;
		Pos(Pos()+size+padding);
	}

	#ifdef LINUX
	#include <byteswap.h>
	#endif

	TUInt16 Swap(const TUInt16& val)
	{
	#if defined LINUX 
		return bswap_16(val);
	#else
		return (val>>8)|(val<<8);
	#endif
	}

	TUInt32 Swap(const TUInt32& val)
	{
	#if defined LINUX 
		return bswap_32(val);
	#else
  	TUInt32 cp = val;
		TByte* ptr=(TByte*) &cp;
		static TByte tmp;
		tmp=ptr[0]; ptr[0]=ptr[3]; ptr[3]=tmp;
		tmp=ptr[1]; ptr[1]=ptr[2]; ptr[2]=tmp;
  	return cp;
	#endif
	}

	TUInt64 Swap(const TUInt64& val)
	{
	#if defined LINUX 
		return bswap_64(val);
	#else
  	TUInt64 cp = val;
		TByte* ptr=(TByte*) &cp;
		static TByte tmp;
		tmp=ptr[0]; ptr[0]=ptr[7]; ptr[7]=tmp;
		tmp=ptr[1]; ptr[1]=ptr[6]; ptr[6]=tmp;
		tmp=ptr[2]; ptr[2]=ptr[5]; ptr[5]=tmp;
		tmp=ptr[3]; ptr[3]=ptr[4]; ptr[4]=tmp;
  	return cp;
	#endif
	}

	void File::_FixByteOrder(
		TByte* ptr,TUInt32 nElems,TUInt32 elemSize)
	{
		switch (elemSize)
		{
			case 1: return;
			case 2:
			{
				TUInt16* fptr = (TUInt16*) ptr;
				for (TUInt32 i=0;i<nElems;i++)
				{
					*fptr = Swap(*fptr);
					fptr++;
				}
				return;
			}
			case 4:
			{
				TUInt32* fptr = (TUInt32*) ptr;
				for (TUInt32 i=0;i<nElems;i++)
				{
					*fptr = Swap(*fptr);
					fptr++;
				}
				return;
			}
			case 8:
			{
				TUInt64* fptr = (TUInt64*) ptr;
				for (TUInt32 i=0;i<nElems;i++)
				{
					*fptr = Swap(*fptr);
					fptr++;
				}
				return;
			}
			default: throw;
		}
	}

	void File::ReadMatrixData(Matrix& matrix)
	{
  	TByte dum[8];
		TUInt32 nElems = matrix.mHeader.mnColumns*matrix.mHeader.mnRows;
		TUInt32 elemSize = matrix.mHeader.mDataType&0xFF;
		TUInt32 size = nElems*elemSize;

		TUInt32 padding = 8-size&7;

	// Testing XA	matrix.mpData = new TByte[size];
		matrix.mpFloat32Data.Resize(nElems);
		matrix.mpFloat32Data.SetSize(nElems);

	// Testing XA	DataFileIO::Read((TByte*) matrix.mpData,size);
		DataFileIO::Read((TByte*) matrix.mpFloat32Data.GetPtr(),size);
	// Testing XA	FixByteOrder((TByte*) matrix.mpData,nElems,elemSize);
		FixByteOrder((TByte*) matrix.mpFloat32Data.GetPtr(),nElems,elemSize);

	//  Pos(Pos()+padding);
		DataFileIO::Read(dum,padding);  
	}

	void File::WriteMatrixData(const Matrix& matrix)
	{
  	TByte dum[8];
		TUInt32 nElems = matrix.mHeader.mnColumns*matrix.mHeader.mnRows;
		TUInt32 elemSize = matrix.mHeader.mDataType&0xFF;
		TUInt32 size = nElems*elemSize;

		TUInt32 padding = 8-size&7;

		TByte tmp[1024];
		const TByte *ptr = (const TByte*) matrix.mpFloat32Data.GetPtr();
		while (size)
		{
			int blocksize = size;
			if (blocksize>1024) blocksize = 1024;
			memcpy(tmp,ptr,blocksize);

			FixByteOrder(tmp,blocksize,elemSize);
			DataFileIO::Write((TByte*) tmp,size);

			ptr+=blocksize;
			size-=blocksize;
		}

		DataFileIO::Write(dum,padding);  
	}

}
