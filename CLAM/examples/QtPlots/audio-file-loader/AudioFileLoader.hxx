#ifndef _AUDIOFILELOADER_
#define _AUDIOFILELOADER_

#include "Audio.hxx"

using CLAM::Audio;

class AudioFileLoader
{
public:
	AudioFileLoader();
	~AudioFileLoader();

	int Load(const char* fileName,Audio& out);
};

#endif
