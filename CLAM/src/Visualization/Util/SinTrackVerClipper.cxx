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
				CLAM_ASSERT( f_upper - f_lower < mMinFreqRange, "Given bounds distance is below the minimum displayable freq range" );
				
				if ( pl_s.empty() && pl_e.empty() )
						return;
				
				CLAM_ASSERT( pl_s.empty(), "Polyline starts table was empty but the ends table was not" );
				CLAM_ASSERT( pl_e.empty(), "Polyline ends table was empty but the starts table was not" );

				CLAM_ASSERT( pl_s.size() != pl_e.size(), "Starts table and ends table sizes differ" );
				
				
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
											SineTrackSpanEnds pl_s&, SineTrackSpanEnds& pl_e  )
		{
				peak_iterator curr_p0 = si;
				peak_iterator curr_p1 = curr_p0;
				peak_iterator old_ei = ei;
				si--;

				bool new_span_start = false;
				bool current_span_finished = false;
				
				outcode outcode0, outcode1;

				outcode0 = in_out_test( f_lower, f_upper, curr_p0->mFreq )

				while ( !current_span_finished && curr_p1!=ei )
				{
						curr_p1++;
						outcode1 = in_out_test( f_lower, f_upper, curr_p1->mFreq );

						//if ( ~outcode0 & ~outcode1  ) // p0 and p1 are inside
						//{
								// does nothing
						//}
						if ( outcode0 & outcode1 ) // p0 and p1 are outside 
						{
								si = curr_p1;
						}
						else if ( outcode0 & Outside ) // p0 is outside, p1 is inside
						{
								if ( !new_span_start ) si = curr_p1;
								else
								{
										pl_s.push_back( curr_p1 );
										pl_e.push_back( old_ei );
										current_span_finished = true;
								}
						}
						else if ( outcode1 & Outside ) // p1 is outside, p0 is inside
						{
								if ( !new_span_start)
								{
										new_span_start = true;
										ei = curr_p0;
								}
								else
								{
										CLAM_DEBUG_ASSERT( false, "Check the algorithm" );
								}								
						}
				
						curr_p0 = curr_p1;
						outcode0 = outcode1;

				}
		}

}
