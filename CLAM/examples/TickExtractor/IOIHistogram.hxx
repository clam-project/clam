#ifndef __IOI_HISTOGRAM__
#define __IOI_HISTOGRAM__

#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		class IOIHistogram : public ProcessingData
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE( IOIHistogram, 2, ProcessingData );
			
			DYN_ATTRIBUTE( 0, public, DataArray, Bins );
			DYN_ATTRIBUTE( 1, public, TData,     BinRate );

		protected:
			
			void DefaultInit();			
		};
	}
	
}

#endif // IOIHistogram.hxx
