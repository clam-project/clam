#ifndef __OGGVORBISAUDIOSTREAM__
#define __OGGVORBISAUDIOSTREAM__

#include "AudioCodecs_Stream.hxx"
#include <string>

namespace CLAM
{

namespace AudioCodecs
{
	class OggVorbisAudioStream : public Stream
	{
	public:
		OggVorbisAudioStream();
		OggVorbisAudioStream( const AudioFile& file );

		~OggVorbisAudioStream();

		void SetFOI( const AudioFile& file );

		void PrepareReading();
		void PrepareWriting();
		void PrepareReadWrite();
		void Dispose();

	protected:
		void AudioFileToNative();
		void DiskToMemoryTransfer();
		void MemoryToDiskTransfer();
		
	protected:
		std::string   mName;
	};
}

}

#endif // OggVorbisAudioStream.hxx
