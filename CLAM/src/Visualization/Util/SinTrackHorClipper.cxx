#include "SinTrackHorClipper.hxx"
#include <algorithm>

namespace CLAMGUI
{
	
		void SinTrackHorClipper::Cull( TIndex start_frame, TIndex end_frame, 
									std::vector< SinusoidalTrack::iterator >& line_starts,
									std::vector< SinusoidalTrack::iterator >& line_ends )
		{
				// Some defensive programming...
				
				CLAM_DEBUG_ASSERT( end_frame > start_frame, "End frame must be greater than start Frame\n" );
				CLAM_DEBUG_ASSERT( ( end_frame - start_frame ) >= 3, "Three frames span is the minimum meaningful\n" ); 


				// vectors resizing checking
				// the policy is simple: never shrink


				TSize ntracks = mList.size();

				if ( ntracks > line_starts.size() )
						line_starts.resize( ntracks );
				if ( ntracks > line_ends.size() )
						line_ends.resize( ntracks );

				SineTrackList::iterator  currentSineTrack, listEnd;

				currentSineTrack = mList.begin();
				listEnd = mList.end();
				
				SinusoidalTrack::iterator currentSpanStart, currentSpanEnd;
				
				currentSpanStart = line_starts.begin();
				currentSpanEnd = line_ends.begin();

				while ( currentSineTrack != listEnd )
				{
					
						DetermineSpanPeaks( start_frame, end_frame, currentSineTrack, currentSpanStart, currentSpanEnd );

						currentSineTrack++;
						currentSpanStart++;
						currentSpanEnd++;
				} 
		}

		void SinTrackHorClipper::DetermineSpanPeaks( TIndex frame0, TIndex frame1, SineTrackList::iterator itrack,
												  peak_iterator& start, peak_iterator& end )
		{
				// "Best" case: the span can be trivially rejected
				
				if ( itrack->front() > frame1 ) // track is to the right of the spanned frames
				{
						start = end = itrack->end(); // void span
						return;						
				}

				if ( itrack->back() < frame0 ) // track is to left of the spanned frames
				{
						start = end = itrack->end();
						return;
				}

				// "Worst" case: the span is somewhere inside the list

				SinusoidalTrack::iterator nodeStart = std::find( itrack->begin(), itrack->end(), frame0 );
								
				start = nodeStart;
				
				SinusoidalTrack::iterator nodeEnd = std::find( nodeStart, itrack->end(), frame1 );
				
				if ( nodeEnd == i->end() ) 
						nodeEnd--; // points to the last node in the list

				end = nodeEnd;
		}
		
}
