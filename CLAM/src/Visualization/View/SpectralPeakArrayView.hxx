#ifndef __SPECTRALPEAKARRAYVIEW__
#define __SPECTRALPEAKARRAYVIEW__

#include "ProcessingDataView.hxx"
#include "SpectralPeakArrayViewAspect.hxx"

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

		class SpectralPeakArrayView : public ProcessingDataView
		{
				// attributes
		private:
				const SpectralPeakArray*        mObserved;
				SpectralPeakArrayViewAspect     mAspect;

		protected:
				Array< Partial >  mPartialBuffer;

				// implementation details
		protected:

				virtual bool TranscribePeakArray( const SpectralPeakArray& );
				
				//class interface
		public:
				SpectralPeakArrayView();

				virtual ~SpectralPeakArrayView();

				virtual const char* GetClassName() const
				{
						return "SpectralPeakArrayView";
				}
				
				virtual Aspect& GetAspect()
				{
						return mAspect;
				}

				virtual bool Refresh();

				virtual bool BindTo( const ProcessingData* procDataObj );
		};
}

#endif // SpectralPeakArrayView.hxx
