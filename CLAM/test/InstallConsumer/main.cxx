#include <CLAM/CLAMVersion.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/AudioMixer.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <iostream>
#include <memory>

// Uses classes from clam_processing and clam_audioio, and checks that the
// installed shared libraries registered their processings in the factory.
int main()
{
	std::cout << "CLAM " << CLAM::GetFullVersion() << std::endl;
	CLAM::AudioMixer mixer;
	CLAM::MonoAudioFileReader reader;
	std::cout << mixer.GetClassName() << ", " << reader.GetClassName() << std::endl;
	CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
	for (const char * key : {"AudioMixer", "MonoAudioFileReader"})
	{
		std::unique_ptr<CLAM::Processing> processing(factory.CreateSafe(key));
		std::cout << key << " -> " << processing->GetClassName() << std::endl;
	}
	return 0;
}
