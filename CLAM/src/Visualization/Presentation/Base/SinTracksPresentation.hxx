#ifndef __SINTRACKSPRESENTATION__
#define __SINTRACKSPRESENTATION__

#include "Presentation.hxx"
#include "Partial.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv2.hxx"
#include "Slotv1.hxx"
#include "SineTracksDef.hxx"

namespace CLAMVM
{
		using SigSlot::Slotv1;
		using SigSlot::Slotv2;
		using CLAM::Array;
		using CLAM::TData;
		using CLAM::TTime;
		using CLAM::TSize;

		class SinTracksModel;
		class SinTracksPresentation : public Presentation
		{
		protected:
				
				virtual void OnNewTrackList( SineTrackList& , TSize ) = 0; 
				
				virtual void OnNewRange( TData ) = 0;

				virtual void OnNewDuration( TTime begin, TTime end ) = 0;
		public:

				SinTracksPresentation();
				~SinTracksPresentation();

				virtual void AttachTo( SinTracksModel& );
				virtual void Detach();

				virtual void Show() = 0;
				virtual void Hide() = 0;

				Slotv2<SineTrackList&, TSize >          SetTrackList;
				Slotv1<TData>                           SetSpectralRange;
				Slotv2<TTime, TTime>                    SetTimeInterval;
		};
}

#endif // SinTracksPresentation.hxx
