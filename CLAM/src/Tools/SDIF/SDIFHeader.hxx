#ifndef __SDIFHeader__
#define __SDIFHeader__

#include "SDIFType.hxx"

namespace SDIF
{

	/**
	* Base header for all frames. 
	* The only frame that uses this header directly is the special 
	* OpeningFrame.
	*
	* @see DataFrameHeader
	*/

	class FrameHeader
	{
	public:
		//* Identifier indicating the kind of frame */
		Type mType;
		//* Size in bytes, of the frame, not including the type or size
		TInt32 mSize;
	public:
		/** Create a new FrameHeader
		* @param type Identifier specifying the frame type
		*/
		FrameHeader(const Type& type);
	};

	/**
	*       An extension of FrameHeader, that is used for data frames.
	*       This is the header that is used for a Frame.
	*/
	class DataFrameHeader:public FrameHeader 
	{
	public:
		 //* Timestamp
		TFloat64 mTime;
		//* Stream ID @see Stream
		TInt32 mStreamId;
		//* Number of data matrixes in the frame. @see Matrix
		TInt32 mnMatrices;
	public:
		/** Create a new DataFrameHeader
		* @param type Identifier specifying the frame type
		* @param time Timestamp of the frame
		* @param id Stream ID
		*/
		DataFrameHeader(const Type& type,TFloat64 time=0.,TInt32 id=0);
	};

	/** The header for each Matrix
	*/
	class MatrixHeader
	{
	public:
	
		//* Identifier indicating the matrix type
		Type mType;
		//* Identifier indicating the type of data in the matrix
		DataType mDataType;
		//* Number of rows in the matrix
		TInt32 mnRows;
		//* Number of columns in the matrix
		TInt32 mnColumns;

	public:
		/** Create a new MatrixHeader
		* @param type Identifier specifying the matrix type
		* @param dataType Identifier specifying the type of data type in the matrix
		* @param nRows Number of rows in the matrix
		* @param nColumns Number of rows in the matrix
		*/
		MatrixHeader(
			const Type& type,
			DataType dataType,
			TInt32 nRows, TInt32 nColumns);
	};

}

#endif
