#include "StdioSinTracksPresentation.hxx"
#include "SinTracksViewAspect.hxx"
#include "SinTrackSegment.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMGUI
{
		void StdioSinTracksPresentation::dump_to_stdout::operator()( const SinusoidalTrack& st )
		{

				if ( st.empty() )
				{
						std::cout << "(WW) Track #" << real_track_id << " was empty!" << std::endl;

						return;
				}
				
				SinusoidalTrack::const_iterator i, end;
				i = st.begin();
				end = st.end();

				std::cout << "Track #" << real_track_id << std::endl;
				
				while ( i != end )
				{
						if ( i->isValid() )
						{
								std::cout << "(" << i->mFreq << "," << i->mTimeIndex << "," << i->mMag << ") ";
						}
						
						i++;
				}
				std::cout << "   " << std::endl;
				
				real_track_id++;

		}

		
		StdioSinTracksPresentation::StdioSinTracksPresentation()
				: mTrackListBuilder( mSineTracks )
		{
		}

		StdioSinTracksPresentation::~StdioSinTracksPresentation()
		{
		}

		void StdioSinTracksPresentation::Bind( Aspect& a ) throw( std::bad_cast )
		{
				SinTracksViewAspect& viewAspect = dynamic_cast< SinTracksViewAspect& >( a );
				
				viewAspect.AcquirePartials.Connect( this, &StdioSinTracksPresentation::HandleIncomingPartials, mPartialsSlot );
				viewAspect.AcquireSpectralRange.Connect( this, &StdioSinTracksPresentation::HandleIncomingRange, mSpectralRangeSlot );
				viewAspect.AcquireDuration.Connect( this, &StdioSinTracksPresentation::HandleIncomingDuration, mDurationSlot );
		}

		void StdioSinTracksPresentation::Show()
		{

				// we will dump the present tracks as well as the valid segments present in each track
			decimate_sine_tracks        Decimate( 3 );

			std::cout << "DECIMATION... ";
	
			Decimate( mSineTracks );

			std::cout << "DONE!" << std::endl;
	
			std::for_each( mSineTracks.begin(), mSineTracks.end(), dump_to_stdout() );
				
		}
		
		void StdioSinTracksPresentation::HandleIncomingRange( TData spec_rng )
		{
				mSpectralRange = spec_rng;
		}

		void StdioSinTracksPresentation::HandleIncomingDuration( TTime begin, TTime end )
		{
				mBeginTime = begin;
				mEndTime = end;
				mLen = end - begin;
		}

		void StdioSinTracksPresentation::HandleIncomingPartials( const Array<Partial>& array, TIndex frame_idx )
		{
				// ufff

				mTrackListBuilder.AddFrame( array, frame_idx );
				std::cout << ".";
		}
		
		
}
