#ifndef __AUDIOCODEC__
#define __AUDIOCODEC__

#include <string>

namespace CLAM
{

	class AudioFile;
	class AudioFileHeader;

namespace AudioCodecs
{
	class Stream;

	class Codec
	{
	public:
		virtual          ~Codec();
		static bool      FileExists( std::string filename );
		virtual bool     IsReadable( std::string filename ) const = 0;
		virtual bool     IsWritable( std::string filename, const AudioFileHeader& ) const = 0;
		virtual Stream*  GetStreamFor( const AudioFile& ) = 0;
		virtual void     RetrieveHeaderData( std::string uri, AudioFileHeader& ) = 0;
	};
}

}


#endif // AudioCodec.hxx
