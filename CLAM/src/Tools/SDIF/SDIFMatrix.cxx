#include "SDIFMatrix.hxx"

namespace SDIF
{

	Matrix::Matrix(const TypeId& type,DataType dataType,
		TInt32 nRows, TInt32 nColumns)
	:mHeader(type,dataType,nRows,nColumns)
	{
		TInt32 nElems = nRows*nColumns;
		
		mpFloat32Data.Resize(nElems);
		mpFloat32Data.SetSize(nElems);
	}

}

