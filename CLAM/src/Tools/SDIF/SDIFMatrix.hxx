#ifndef __Matrix__
#define __Matrix__

#include "SDIFHeader.hxx"
#include "Array.hxx"
#include "SDIFType.hxx"

/** The data in a Frames are stored in 2D matrices.
* Each column corresponds to a parameter like frequency or amplitude and 
* each row represents an object like a filter, sinusoid, or noise band.
* Each Matrix starts with a MatrixHeader, that contains a
*       type id, and the dimensions of the matrix.
*
*       As Matrixes are stored in a linked list in the Frames,> *       each Matrix has a pointer to the next matrix inside the
*       frame.
*/
namespace SDIF
{
	class Matrix
	{
	public:
		MatrixHeader mHeader;

		// Testing XA union {
			// Testing XAvoid* mpData;
			// Testing XA TFloat32* mpFloat32Data;
		CLAM::Array<TFloat32> mpFloat32Data;
		// Testing XA};
		
	public:
		/** Create a new Matrix.
		* @param type Identifier specifying the matrix type
		* @param dataType Identifier specifying the type of data type in the matrix
		* @param nRows Number of rows in the matrix
		* @param nColumns Number of rows in the matrix
		*/
		Matrix(
				const TypeId& type = TypeId::sDefault,
				DataType dataType = eUnknown,
				TInt32 nRows = 0, TInt32 nColumns = 0);

		/** Lookup a value in the matrix
		* @param row,col coordinates of the value
		* @return The indicated value
		*/
		TFloat32 GetValue(TInt32 row,TInt32 col)
		{
			return mpFloat32Data[row*mHeader.mnColumns + col];
		}
		
		void SetValue(TInt32 row,TInt32 col,TFloat32 val)
		{
			mpFloat32Data[row*mHeader.mnColumns + col] = val;
		}
		
		TInt32 Rows(void) { return mHeader.mnRows; }
		TInt32 Columns(void) { return mHeader.mnColumns; }
	};
}
#endif
