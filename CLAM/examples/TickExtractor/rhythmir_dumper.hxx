#ifndef __rhythmir_dumper__
#define __rhythmir_dumper__

#include <string>

namespace CLAM
{
	class Pulse;
}

extern void DumpExtractedData( std::string filename, 
			       std::string analyzedFile,
			       const CLAM::Pulse& ticksData,
			       const CLAM::Pulse& beatsData );

#endif // rhythmir_dumper.hxx
