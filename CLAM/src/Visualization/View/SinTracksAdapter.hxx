#ifndef __SINTRACKSADAPTER__
#define __SINTRACKSADAPTER__

#include "SpectralPeakArrayAdapter.hxx"
#include "SinTracksModel.hxx"
#include "SineTracksDef.hxx"
#include "SinTrackListBuilder.hxx"

namespace CLAM
{
	class Segment;
};


namespace CLAMVM
{
		using CLAM::Segment;
		using CLAM::ProcessingData;
		using CLAM::Array;

		class SinTracksAdapter 
				: public SpectralPeakArrayAdapter, public SinTracksModel
		{
				// attributes
		private:
				const Segment*                  mObserved;
				SineTrackList                   mExtractedList;
				SinTrackBuilder                 mTrackBuilder;

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


				virtual bool Publish();
			
				virtual bool BindTo( const ProcessingData& procDataObj );
		};

}
#endif // SinTrackView.hxx
