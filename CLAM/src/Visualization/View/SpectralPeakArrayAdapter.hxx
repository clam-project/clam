#ifndef __SPECTRALPEAKARRAYADAPTER__
#define __SPECTRALPEAKARRAYADAPTER__

#include "ProcessingDataAdapter.hxx"
#include "SpectralPeakArrayAspect.hxx"

namespace CLAM
{
		class SpectralPeakArray;
		class ProcessingData;
}

namespace CLAMGUI
{
		using CLAM::SpectralPeakArray;
		using CLAM::ProcessingData;
		using CLAM::Array;

		class SpectralPeakArrayAdapter : public ProcessingDataAdapter
		{
				// attributes
		private:
				const SpectralPeakArray*        mObserved;
				SpectralPeakArrayAspect         mAspect;

		protected:
				Array< Partial >  mPartialBuffer;

				// implementation details
		protected:

				virtual bool TranscribePeakArray( const SpectralPeakArray& );
				
				//class interface
		public:
				SpectralPeakArrayAdapter();

				virtual ~SpectralPeakArrayAdapter();

				virtual const char* GetClassName() const
				{
						return "SpectralPeakArrayAdapter";
				}
				
				virtual Aspect& GetAspect()
				{
						return mAspect;
				}

				virtual bool Publish();

				virtual bool BindTo( const ProcessingData* procDataObj );
		};
}

#endif // SpectralPeakArrayAdapter.hxx
