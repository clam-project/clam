#ifndef __SINTRACKSADAPTER__
#define __SINTRACKSADAPTER__

#include "SpectralPeakArrayAdapter.hxx"
#include "SinTracksAspect.hxx"

namespace CLAM
{
	class Segment;
};


namespace CLAMGUI
{
		using CLAM::Segment;
		using CLAM::ProcessingData;
		using CLAM::Array;

		class SinTracksAdapter : public SpectralPeakArrayAdapter
		{
				// attributes
		private:
				const Segment*                  mObserved;
		protected:				
				SinTracksAspect             mThisAspect;
				// implementation details
		protected:
				
				virtual void TransmitPeakArrays();

				// class interface
		public:

				SinTracksAdapter();

				virtual ~SinTracksAdapter();

				virtual const char* GetClassName() const
				{
						return "SinTracksAdapter";
				}

				virtual Aspect& GetAspect()
				{
						return mThisAspect;
				}

				virtual bool Publish();
			
				virtual bool BindTo( const ProcessingData* procDataObj );
		};

}
#endif // SinTrackView.hxx
