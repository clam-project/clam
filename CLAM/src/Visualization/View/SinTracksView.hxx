#ifndef __SINTRACKSVIEW__
#define __SINTRACKSVIEW__

#include "SpectralPeakArrayView.hxx"
#include "SinTracksViewAspect.hxx"

namespace CLAM
{
	class Segment;
};


namespace CLAMGUI
{
		using CLAM::Segment;
		using CLAM::ProcessingData;
		using CLAM::Array;

		class SinTracksView : public SpectralPeakArrayView
		{
				// attributes
		private:
				const Segment*                  mObserved;
		protected:				
				SinTracksViewAspect             mThisAspect;
				// implementation details
		protected:
				
				virtual void TransmitPeakArrays();

				// class interface
		public:

				SinTracksView();

				virtual ~SinTracksView();

				virtual const char* GetClassName() const
				{
						return "SinTracksView";
				}

				virtual Aspect& GetAspect()
				{
						return mThisAspect;
				}

				virtual bool Refresh();
			
				virtual bool BindTo( const ProcessingData* procDataObj );
		};

}
#endif // SinTrackView.hxx
