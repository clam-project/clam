#include "AudioFileLoader.hxx"
#include "MonoAudioFileReader.hxx"

using namespace CLAM;

AudioFileLoader::AudioFileLoader(){}
AudioFileLoader::~AudioFileLoader(){}

int AudioFileLoader::Load(const char* fileName,Audio& out)
{
	AudioFile file;
	file.OpenExisting(fileName);

	if((!file.IsReadable()) | (file.GetHeader().GetChannels() > 1))
		return -1; // no es legible o no es mono

	out.SetSize(file.GetHeader().GetSamples());

	MonoAudioFileReaderConfig cfg;
	cfg.SetSourceFile(file);

	MonoAudioFileReader infile;
	infile.Configure(cfg);

	infile.Start();
	infile.Do(out);
	infile.Stop();

	return 0;
}

// END

