#ifndef __TICK_EXTRACTOR__
#define __TICK_EXTRACTOR__

#include <string>
#include "Array.hxx"

namespace CLAM
{
	void ExtractTicksSequence( std::string pathToFile,
				   CLAM::DataArray& extractedTicksSequence );
}

#endif // TickExtractor.hxx
