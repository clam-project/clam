#include "AudioCodec.hxx"
#include "Assert.hxx"
#include <cstdio>

namespace CLAM
{

namespace AudioCodecs
{
	Codec::~Codec()
	{
		
	}

	bool Codec::FileExists( std::string filename )
	{
		CLAM_DEBUG_ASSERT( filename.length() > 0, "Unnamed files never exist" );

		// MRJ: Quite weak file existence check

		FILE* fileHandle = NULL;

		fileHandle = fopen( filename.c_str(), "rb" );
		
		if ( !fileHandle )
			return false;
		
		fclose( fileHandle );

		return true;
	}
}

}
