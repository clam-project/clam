#ifndef __SDIFFile__
#define __SDIFFile__

#include "SDIFType.hxx"
#include "DataFileIO.hxx"
#include "SDIFHeader.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"
#include "SDIFStorage.hxx"
#include "ByteOrder.hxx"

/** Used to read or write an SDIF file. When reading, the File
* parses the whole files and passes the read Frames to a 
* Storage, typically a Collection to store all in memory.
*       
*       Note that the Storage is responsable for the 
*       freeing memory of the read frames, though File
*       creates them.
*       
*       SDIF files use BIG ENDIAN byte order for the data storage.
*       SDIFFile takes care of fixing the byte order in case
*       the operating system is not BIG ENDIANm, @see CLAMByteOrder
*/

using namespace CLAM;

namespace SDIF
{

class File:public DataFileIO
{
private:
	bool mSkipData;
	bool mFirstAccess;
public:
	File(const char* filename, Mode mode);
	~File();

private:
	inline void FixByteOrder(TByte* ptr,
		TUInt32 nElems,TUInt32 elemSize);
	
	void Read(TInt32& t);
	void Write(const TInt32& t);

	void Read(TFloat64& t);
	void Write(const TFloat64& t);

	void Read(TypeId& header);
	void Write(const TypeId& header);

	void Read(DataFrameHeader& header);
	void Write(const DataFrameHeader& header);

	void Read(FrameHeader& header);
	void Write(const FrameHeader& header);

	void Read(MatrixHeader& header);
	void Write(const MatrixHeader& header);

	void Read(OpeningsFrame& frame);
	void Write(const OpeningsFrame& frame);

	void Read(Matrix& matrix);
	void Write(const Matrix& matrix);

	void SkipMatrixData(const Matrix& matrix);
	void ReadMatrixData(Matrix& matrix);
	void WriteMatrixData(const Matrix& matrix);

public:
	void Read(Frame& frame);
	void Write(const Frame& frame);

	void Read(Storage& storage);
	void Write(const Storage& storage);
private:
	void _FixByteOrder(
	TByte* ptr,TUInt32 nElems,TUInt32 elemSize);
};

inline void File::Read(TInt32& t)
{
	DataFileIO::Read(t);
	FixByteOrder((TByte*) &t,1,sizeof(t));
}

inline void File::Read(TFloat64& t)
{
	DataFileIO::Read(t);
	FixByteOrder((TByte*) &t,1,sizeof(t));
}

inline void File::Write(const TInt32& t)
{
	TInt32 tmp(t);
	FixByteOrder((TByte*) &tmp,1,sizeof(tmp));
	DataFileIO::Write(tmp);
}

inline void File::Write(const TFloat64& t)
{
	TFloat64 tmp(t);
	FixByteOrder((TByte*) &tmp,1,sizeof(tmp));
	DataFileIO::Write(tmp);
}

inline void File::FixByteOrder(TByte* ptr,
	TUInt32 nElems,TUInt32 elemSize)
{
#ifdef CLAM_LITTLE_ENDIAN
	_FixByteOrder(ptr,nElems,elemSize);
#else
#ifndef CLAM_BIG_ENDIAN
#pragma message ("BYTE ORDER NOT DEFINED!")
#endif
#endif
}

}

#endif
