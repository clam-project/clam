#ifndef __TICK_EXTRACTOR__
#define __TICK_EXTRACTOR__

#include <string>
#include "Array.hxx"
#include "Pulse.hxx"

namespace CLAM
{
	void ExtractTicksSequence( std::string pathToFile,
				   CLAM::Pulse& ticksList);
}

#endif // TickExtractor.hxx
