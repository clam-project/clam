#ifndef __PLOTSINTRACKS__
#define __PLOTSINTRACKS__

// forward declaration
namespace CLAM
{
	class Segment;
}

namespace CLAMVM
{
	/**
	 *  Overload of plot() for CLAM::Segment
	 */
	void plot( const CLAM::Segment& model, const char* label = NULL );
}


#endif // PlotSinTracks.hxx
