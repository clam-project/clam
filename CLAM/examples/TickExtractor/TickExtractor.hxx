#ifndef __TICK_EXTRACTOR__
#define __TICK_EXTRACTOR__

#include <string>
#include "Array.hxx"
#include "Pulse.hxx"

namespace CLAM
{
	class TickExtractorConfig;

	void ExtractTicksSequence( std::string pathToFile,
				   CLAM::Pulse& ticksList );

	void ExtractTicksSequence( std::string pathToFile,
				   TickExtractorConfig& cfg,
				   CLAM::Pulse& ticksList);
}

#endif // TickExtractor.hxx
