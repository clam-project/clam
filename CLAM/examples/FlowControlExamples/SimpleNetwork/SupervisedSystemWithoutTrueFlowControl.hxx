#ifndef _SupervisedSystemWithoutTrueFlowControl_hxx_
#define _SupervisedSystemWithoutTrueFlowControl_hxx_

#include "AudioManager.hxx"
#include "Network.hxx"
#include "Oscillator.hxx"
#include "AudioFile.hxx"
#include "AudioOut.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"
#include "AudioMixer.hxx"
#include <list>

namespace FlowControlExample
{

class SupervisedSystemWithoutTrueFlowControl
{
	typedef std::list<CLAM::Network*> NetworkList;
public:	
	SupervisedSystemWithoutTrueFlowControl( 
		std::string fileIn, 
		std::string fileOut, 
		int frameSize, 
		int maxFramesToProcess, 
		bool hasAudioOut );

	virtual ~SupervisedSystemWithoutTrueFlowControl(){}

	void ConfigureNetworks();
	void ProcessAllNetworks();

private:
	void InitConfigurations();
	void ConfigureOscillatorToFileOut();
	void ConfigureFileInFileOut();
	void ConfigureModulatedFileIn();
	void ConfigureModulatedOscillator();
	void ConfigureModulatedFileInPlusFileIn();


	//other system parameters
	std::string _fileInName;
	std::string _fileOutName;
	int _frameSize;
	int _maxFramesToProcess;
	bool _hasAudioOut;

	//networks
	CLAM::Network _oscillatorToFileOut;
	CLAM::Network _fileInFileOut;
	CLAM::Network _modulatedFileIn;
	CLAM::Network _modulatedOscillator;
	CLAM::Network _modulatedFileInPlusFileIn;

	NetworkList _networks;

	CLAM::AudioManager _audioManager;

	//configurations
	CLAM::OscillatorConfig _generatorCfg;
	CLAM::OscillatorConfig _modulatorCfg;
	CLAM::AudioFileConfig _fileInCfg;
	CLAM::AudioFileConfig _fileOutCfg;
	CLAM::AudioIOConfig _audioOutCfg;
	CLAM::AudioMixerConfig _mixerCfg;
};

} //namespace

#endif
