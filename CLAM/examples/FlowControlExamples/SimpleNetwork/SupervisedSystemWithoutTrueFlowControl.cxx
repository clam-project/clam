
#include "SupervisedSystemWithoutTrueFlowControl.hxx"

FlowControlExample
{
SupervisedSystemWithoutTrueFlowControl::SupervisedSystemWithoutTrueFlowControl
	( std::string fileIn, std::string fileOut , int frameSize, int maxFramesToProcess, bool hasAudioOut ):
	_audioManager(44100, frameSize),
	_frameSize(frameSize), 
	_maxFramesToProcess(nFrames),
	_hasAudioOut(hasAudioOut),

{
	ConfigureNetworks();
}

void SupervisedSystemWithoutTrueFlowControl::ConfigureNetworks()
{
	ConfigureOscillatorToFileOut();
	ConfigureFileInFileOut();
	ConfigureModulatedFileIn();
	ConfigureModulatedOscillator();
	ConfigureModulatedFileInPlusFileIn();	
}

void SupervisedSystemWithoutTrueFlowControl::ConfigureOscillatorToFileOut()
{	

	// create the processing
	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);
	oscilCfg.SetName("Oscillator0");

	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);
	fileCfg.SetName("File Out");

	_oscillatorToFileOut.AddProcessing(new CLAM::Oscillator(oscilCfg));
	_oscillatorToFileOut.AddProcessing(new CLAM::AudioFileOut(fileCfg));

	//link them
	_oscillatorToFileOut.AddConnection( "Oscillator0", "Audio Output", "File Out", "Input" );

	if (_hasAudioOut)
	{
		fileCfg.SetName("Audio Out");
		_oscillatorToFileOut.AddProcessing(new CLAM::AudioOut(fileCfg));
		_oscillatorToFileOut.AddConnection( "Oscillator0", "Audio Output", "Audio Out", "Input" );
	}

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut()
{
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn()
{
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedOscillator()
{
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileInPlusFileIn()
{
}

void SupervisedSystemWithoutTrueFlowControl::ProcessAllNetworks()
{
	
}

} //namespace


