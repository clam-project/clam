#ifndef __SINTRACKHORCLIPPER__
#define __SINTRACKHORCLIPPER__

#include "SineTracksDef.hxx"
#include <vector>
#include "DataTypes.hxx"

namespace CLAMGUI
{
		
		using CLAM::TIndex;
		using CLAM::TSize;

		class SinTrackHorClipper
		{
				typedef std::vector< SinusoidalTrack::iterator>::iterator peak_iterator;
		public:
				
				SinTrackHorClipper( SineTrackList& list )
						: mList( list )
				{
				}

				void Cull( TIndex start_frame, TIndex end_frame, 
						   std::vector< SinusoidalTrack::iterator >& line_starts,
						   std::vector< SinusoidalTrack::iterator >& line_ends );

		protected:
				
				void DetermineSpanPeaks( TIndex frame0, TIndex frame1, SineTrackList::iterator itrack,
										 peak_iterator& start, peak_iterator& end );

		private:
				
				SineTrackList&   mList;
		};
}


#endif // CullSineTracks.hxx
