#include "SinTracksAdapter.hxx"
#include "Segment.hxx"
#include "Frame.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAMVM
{
		using CLAM::Frame;
		using CLAM::SpectralPeakArray;
		using CLAM::TSize;
		
		SinTracksAdapter::SinTracksAdapter()
				: mObserved( NULL )
		{
				mTrackBuilder.SetTrackList( mExtractedList );

		}

		SinTracksAdapter::~SinTracksAdapter()
		{
		}

		bool SinTracksAdapter::BindTo( const ProcessingData& obj )
		{
				mObserved = dynamic_cast< const Segment* >( &obj );

				CLAM_ASSERT( mObserved->HasSamplingRate(), "Not a valid segment: it doesn't know its sampling rate " );
				CLAM_ASSERT( mObserved->HasBeginTime(), "Not a valid segment: it doesn't know its begin time " );
				CLAM_ASSERT( mObserved->HasEndTime(), "Not a valid segment: it doesn't know its end time " );

				if ( !mObserved ) return false;

				// new object, new list
				mTrackBuilder.Flush();

				return true;
		}

		bool SinTracksAdapter::Publish()
		{
				if ( !mObserved )
						return false;

				TransmitPeakArrays();
				
				SpectralRangePublished.Emit( mObserved->GetSamplingRate()/2 );
				DurationPublished.Emit( mObserved->GetBeginTime(), mObserved->GetEndTime() );

				return true;
		}

		void SinTracksAdapter::TransmitPeakArrays()
		{
				TSize numFrames = mObserved->GetnFrames();
				TSize idx = 0;

				while ( idx < numFrames )
				{
						const Frame& f = mObserved->GetFrame( idx );
						
						if ( TranscribePeakArray( f.GetSpectralPeakArray() ) )
							mTrackBuilder.AddFrame( mPartialBuffer, idx );
						
						idx++;
				}
				
				TrackListPublished.Emit( mExtractedList, numFrames );
				
		}
}
