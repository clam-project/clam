#include "SinTracksView.hxx"
#include "Segment.hxx"
#include "Frame.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAMGUI
{
		using CLAM::Frame;
		using CLAM::SpectralPeakArray;
		using CLAM::TSize;
		
		SinTracksView::SinTracksView()
				: mObserved( NULL ), mThisAspect( *this )
		{
		}

		SinTracksView::~SinTracksView()
		{
		}

		bool SinTracksView::BindTo( const ProcessingData* obj )
		{
				mObserved = dynamic_cast< const Segment* >( obj );

				CLAM_ASSERT( mObserved->HasSamplingRate(), "Not a valid segment: it doesn't know its sampling rate " );
				CLAM_ASSERT( mObserved->HasBeginTime(), "Not a valid segment: it doesn't know its begin time " );
				CLAM_ASSERT( mObserved->HasEndTime(), "Not a valid segment: it doesn't know its end time " );

				if ( !mObserved ) return false;

				return true;
		}

		bool SinTracksView::Refresh()
		{
				if ( !mObserved )
						return false;

				TransmitPeakArrays();
				
				mThisAspect.AcquireSpectralRange.Emit( mObserved->GetSamplingRate() );
				mThisAspect.AcquireDuration.Emit( mObserved->GetBeginTime(), mObserved->GetEndTime() );

				return true;
		}

		void SinTracksView::TransmitPeakArrays()
		{
				TSize numFrames = mObserved->GetnFrames();
				TSize idx = 0;

				while ( idx < numFrames )
				{
						const Frame& f = mObserved->GetFrame( idx );
						
						if ( TranscribePeakArray( f.GetSpectralPeakArray() ) )
							mThisAspect.AcquirePartials.Emit( mPartialBuffer, idx );
						
						idx++;
				}
				
		}
}
