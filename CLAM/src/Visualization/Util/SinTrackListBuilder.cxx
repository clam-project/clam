#include "SinTrackListBuilder.hxx"
#include <algorithm>

namespace CLAMGUI
{
		SinTrackBuilder::SinTrackBuilder( SineTrackList& l )
				: mSineTrackList( l )
		{
		}

		void SinTrackBuilder::AddFrame( const Array<Partial>& a, TIndex frame_idx )
		{
				for ( int i = 0; i < a.Size(); i++ )
				{
						const Partial& p = a[i];

						if ( p.mTrackId >= 0 )
						{
								
								TrackingList::iterator i = mHeadsList.find(  p.mTrackId  );

								if ( i == mHeadsList.end() ) // new track at the end
								{
										mSineTrackList.push_back( SinusoidalTrack() );								
										mHeadsList.insert( std::make_pair( p.mTrackId, TrackHead( p.mTrackId, frame_idx, &mSineTrackList.back() ) ) );
										
										TrackHead& h = mHeadsList[p.mTrackId];
										h.Update( frame_idx, p );
								}
								else  // exact match
								{
										// liveness check
										TrackHead& h = i->second;
										
										if ( h.IsAlive( frame_idx ) )
										{
												h.Update( frame_idx, p );
										}
										else  // it was dead, so we have to "resurrect" it
										{
												mSineTrackList.push_back( SinusoidalTrack() );
												h.AssignList( &mSineTrackList.back() );
												h.Update( frame_idx, p );
										}
								}
						}
				}

		}
		
}
