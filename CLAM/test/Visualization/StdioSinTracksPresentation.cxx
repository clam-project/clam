#include "StdioSinTracksPresentation.hxx"
#include "SinTracksAspect.hxx"
//#include "SinTrackSegment.hxx"
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
				SetPartials.Wrap( this, &StdioSinTracksPresentation::OnNewPartials );
				SetSpectralRange.Wrap( this, &StdioSinTracksPresentation::OnNewRange );
				SetDuration.Wrap( this, &StdioSinTracksPresentation::OnNewDuration );
		}

		StdioSinTracksPresentation::~StdioSinTracksPresentation()
		{
		}

		void StdioSinTracksPresentation::Bind( Aspect& a ) throw( std::bad_cast )
		{
				SinTracksAspect& viewAspect = dynamic_cast< SinTracksAspect& >( a );
				
				viewAspect.AcquirePartials.Connect( SetPartials );
				viewAspect.AcquireSpectralRange.Connect( SetSpectralRange );
				viewAspect.AcquireDuration.Connect( SetDuration  );
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
		
		void StdioSinTracksPresentation::OnNewRange( TData spec_rng )
		{
				mSpectralRange = spec_rng;
		}

		void StdioSinTracksPresentation::OnNewDuration( TTime begin, TTime end )
		{
				mBeginTime = begin;
				mEndTime = end;
				mLen = end - begin;
		}

		void StdioSinTracksPresentation::OnNewPartials( const Array<Partial>& array, TIndex frame_idx )
		{
				// ufff

				mTrackListBuilder.AddFrame( array, frame_idx );
				std::cout << ".";
		}
		
		
}
