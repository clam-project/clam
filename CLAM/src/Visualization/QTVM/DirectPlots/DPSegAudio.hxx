#ifndef __DPSEGAUDIO__
#define __DPSEGAUDIO__

#include <list>
#include <string>
#include "Audio.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotSegAudio(	const Audio& audio,
							std::list<unsigned>& marks,
							const std::string& label="",
							int x=100, 
							int y=100, 
							int w=500, 
							int h=225 );
	}
}

#endif

