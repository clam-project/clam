#include "SinTrackVerClipper.hxx"
#include "Assert.hxx"

namespace CLAMGUI
{

		SinTrackVerClipper::SinTrackVerClipper( TData minfreqrange )
				: mMinFreqRange( minfreqrange )
		{
		}

		SinTrackVerClipper::~SinTrackVerClipper()
		{
		}

		void SinTrackVerClipper::Cull( TData f_lower, TData f_upper, SineTrackSpanEnds& pl_s, SineTrackSpanEnds& pl_e )
		{
				CLAM_ASSERT( f_upper - f_lower > mMinFreqRange, "Given bounds distance is below the minimum displayable freq range" );
				
				if ( pl_s.empty() && pl_e.empty() )
						return;
				
				CLAM_ASSERT( !pl_s.empty(), "Polyline starts table was empty but the ends table was not" );
				CLAM_ASSERT( !pl_e.empty(), "Polyline ends table was empty but the starts table was not" );

				CLAM_ASSERT( pl_s.size() == pl_e.size(), "Starts table and ends table sizes differ" );
				
				
				// Basic algorithm idea: to check if every node in a given Span is inside the given
				// frequency ( bounds ). If its not, we choose it as the new span end
				//
				// for each node in a given span
				//          if node out of bounds
				//                 push next span index into index stack
				//                 save previous span end
				//                 set the current node as current span end
				//                 add two new entries to the span tables
				//                 search for the first contigous node inside bounds
				//                        if new node found
				//                           set the new span start entry 
				
				int i = 0;
				int nSpans; 				
				do 
				{

						nSpans = pl_s.size(); // we save the original span tables size

						while ( i < nSpans )
						{
								
								ClipSpans( f_lower, f_upper, pl_s[i], pl_e[i], pl_s, pl_e  );
								
								i++;
						}
				} while( pl_s.size() != nSpans );
		}

		void SinTrackVerClipper::ClipSpans( TData f_lower, TData f_upper, peak_iterator& si, peak_iterator& ei, 
											SineTrackSpanEnds& pl_s, SineTrackSpanEnds& pl_e  )
		{
				if ( si == ei )
						return;

				peak_iterator currp = si;
				peak_iterator listEnd = ei;

				peak_iterator first_in = find_first_in( f_lower, f_upper, currp, listEnd );
				
				if ( first_in == ei ) // no points in
				{
						si = ei;
						return;
				}
				
				si = currp = first_in; // we set the start to the first_in ( as well as the current point )

				peak_iterator last_in = find_last_in( f_lower, f_upper, currp, listEnd );
				
				ei = currp = last_in; // first span 

				while ( currp!= listEnd )
				{
						currp++;
						peak_iterator new_start;
						peak_iterator new_end;

						new_start = currp  = find_first_in( f_lower, f_upper, currp, listEnd );
						if ( new_start == listEnd ) return;
						new_end = currp = find_last_in( f_lower, f_upper, currp, listEnd );
						pl_s.push_back( new_start );
						pl_e.push_back( new_end );
				}

		}

}
