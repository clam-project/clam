#include "Header.hxx"

namespace SDIF
{

	FrameHeader::FrameHeader(
		const Type& type)
	{
		mType = type;
		mSize = 0;
	}

	DataFrameHeader::DataFrameHeader(const Type& type,TFloat64 time,TInt32 id)
	:FrameHeader(type) 
	{
		mTime = time;
		mStreamId = id;
		mnMatrices = 0;	
	}

	MatrixHeader::MatrixHeader(const Type& type = "\0\0\0\0",DataType dataType = eUnknown,
		TInt32 nRows = 0, TInt32 nColumns = 0)
	{
		mType = type;
		mDataType = dataType;
		mnRows = nRows;
		mnColumns = nColumns;
	}

}
