#ifndef _SupervisedSystemWithoutTrueFlowControl_hxx_
#define _SupervisedSystemWithoutTrueFlowControl_hxx_

#include "AudioManager.hxx"
#include "Network.hxx"

namespace FlowControlExample
{

class SupervisedSystemWithoutTrueFlowControl
{

public:	
	SupervisedSystemWithoutTrueFlowControl
	( std::string fileIn, std::string fileOut , int frameSize, int maxFramesToProcess, bool hasAudioOut );

	virtual ~SupervisedSystemWithoutTrueFlowControl();{}

	void ConfigureNetworks();
	void ProcessAllNetworks();
private:

	void  ConfigureOscillatorToFileOut();
	void  ConfigureFileInFileOut();
	void  ConfigureModulatedFileIn();
	void  ConfigureModulatedOscillator();
	void  ConfigureModulatedFileInPlusFileIn();



	//other system parameters
	std::string _fileInName;
	std::string _fileOutName;
	int _frameSize;
	int _maxFramesToProcess;
	bool _hasAudioOut;

	//networks
	Network _oscillatorToFileOut;
	Network _fileInFileOut;
	Network _modulatedFileIn;
	Network _modulatedOscillator;
	Network _modulatedFileInPlusFileIn;

	CLAM::AudioManager _audioManager;

};

} //namespace

#endif
