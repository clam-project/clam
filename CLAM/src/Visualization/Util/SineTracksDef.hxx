#ifndef __SINETRACKSDEF__
#define __SINETRACKSDEF__

#include <list>
#include "TimeFreqMagPoint.hxx"


namespace CLAMGUI
{

		typedef std::list< TimeFreqMagPoint >      SinusoidalTrack;
		typedef std::list< SinusoidalTrack >       SineTrackList;


}

#endif // SineTracksDef.hxx
