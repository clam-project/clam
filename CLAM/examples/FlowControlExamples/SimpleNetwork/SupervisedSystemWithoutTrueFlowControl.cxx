
#include "SupervisedSystemWithoutTrueFlowControl.hxx"

#include "Oscillator.hxx"
#include "AudioFile.hxx"
#include "AudioOut.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"
#include "AudioMultiplier.hxx"
#include "AudioMixer.hxx"

//#include "Audio.hxx"
//#include "AudioManager.hxx"




namespace FlowControlExample
{

// constructor
SupervisedSystemWithoutTrueFlowControl::SupervisedSystemWithoutTrueFlowControl ( 
	std::string fileIn, 
	std::string fileOut , 
	int frameSize, 
	int maxFramesToProcess, 
	bool hasAudioOut 
)	:
	_audioManager(44100, frameSize),
	_frameSize(frameSize), 
	_maxFramesToProcess(maxFramesToProcess),
	_hasAudioOut(hasAudioOut),
	_fileInName(fileIn),
	_fileOutName(fileOut)
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

	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

	_oscillatorToFileOut.AddProcessing("oscillator-generator", new CLAM::Oscillator(oscilCfg) );
	_oscillatorToFileOut.AddProcessing("file-out", new CLAM::AudioFileOut(fileCfg));

	//link them
	_oscillatorToFileOut.ConnectPorts( "oscillator-generator.Audio Output", "file-out.Input" );

	if (_hasAudioOut)
	{

		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);

		_oscillatorToFileOut.AddProcessing( "audio-out", new CLAM::AudioOut(audioCfg) );
		_oscillatorToFileOut.ConnectPorts( "oscillator-generator.Audio Output", "audio-out.Input" );
	}

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut()
{

	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

	_fileInFileOut.AddProcessing("file-out", new CLAM::AudioFileOut(fileCfg));
	
	fileCfg.SetFilename( _fileInName );

	_fileInFileOut.AddProcessing("file-in", new CLAM::AudioFileIn(fileCfg));

	_fileInFileOut.ConnectPorts( "file-in.Output", "file-out.Input" );
	
	if (_hasAudioOut)
	{
		
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);
		
		_fileInFileOut.AddProcessing( "audio-out", new CLAM::AudioOut(audioCfg) );
		_fileInFileOut.ConnectPorts( "file-in.Output", "audio-out.Input" );
	}

	

}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn()
{
	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

	_modulatedFileIn.AddProcessing("file-out", new CLAM::AudioFileOut(fileCfg));
	
	fileCfg.SetFilename( _fileInName );

	_modulatedFileIn.AddProcessing("file-in", new CLAM::AudioFileIn(fileCfg));

	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);
	
	_modulatedFileIn.AddProcessing("oscillator-modulator", new CLAM::Oscillator(oscilCfg) );

	_modulatedFileIn.AddProcessing("multiplier", new CLAM::AudioMultiplier );

	_modulatedFileIn.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
	_modulatedFileIn.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_modulatedFileIn.ConnectPorts( "multiplier.Audio Output", "file-out.Input" );	

	if (_hasAudioOut)
	{
		
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);
		
		_modulatedFileIn.AddProcessing( "audio-out", new CLAM::AudioOut(audioCfg) );
		_modulatedFileIn.ConnectPorts( "multiplier.Audio Output", "audio-out.Input" );
	}

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedOscillator()
{
	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

	_modulatedOscillator.AddProcessing("file-out", new CLAM::AudioFileOut(fileCfg));
	
	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);
	
	_modulatedOscillator.AddProcessing( "oscillator-generator", new CLAM::Oscillator(oscilCfg) );

	oscilCfg.SetFrequency(220.0);
	_modulatedOscillator.AddProcessing( "oscillator-modulator", new CLAM::Oscillator(oscilCfg) );

	_modulatedOscillator.AddProcessing( "multiplier", new CLAM::AudioMultiplier );

	_modulatedOscillator.ConnectPorts( "oscillator-generator.Audio Output", "multiplier.First Audio Input" );
	_modulatedOscillator.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_modulatedOscillator.ConnectPorts( "multiplier.Audio Output", "file-out.Input" );	

	if (_hasAudioOut)
	{
		
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);
		
		_modulatedOscillator.AddProcessing( "audio-out", new CLAM::AudioOut(audioCfg) );
		_modulatedOscillator.ConnectPorts( "multiplier.Audio Output", "audio-out.Input" );
	}
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileInPlusFileIn()
{
	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

	_modulatedFileInPlusFileIn.AddProcessing("file-out", new CLAM::AudioFileOut(fileCfg));
	
	fileCfg.SetFilename( _fileInName );

	_modulatedFileInPlusFileIn.AddProcessing("file-in", new CLAM::AudioFileIn(fileCfg));

	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);
	
	_modulatedFileInPlusFileIn.AddProcessing("oscillator-modulator", new CLAM::Oscillator(oscilCfg) );

	_modulatedFileInPlusFileIn.AddProcessing("multiplier", new CLAM::AudioMultiplier );

	CLAM::AudioMixerConfig mixerCfg;
	mixerCfg.SetFrameSize(_frameSize);

	_modulatedFileInPlusFileIn.AddProcessing( "mixer", new CLAM::AudioMixer<2>(mixerCfg) );

	_modulatedFileInPlusFileIn.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
	_modulatedFileInPlusFileIn.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_modulatedFileInPlusFileIn.ConnectPorts( "file-in.Output", "mixer.Input Audio_1" );
	_modulatedFileInPlusFileIn.ConnectPorts( "multiplier.Audio Output" , "mixer.Input Audio_0" );
	_modulatedFileInPlusFileIn.ConnectPorts( "mixer.Output Audio", "file-out.Input" );
	
	if (_hasAudioOut)
	{
		
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);
		
		_modulatedFileInPlusFileIn.AddProcessing( "audio-out", new CLAM::AudioOut(audioCfg) );
		_modulatedFileInPlusFileIn.ConnectPorts( "mixer.Output Audio", "audio-out.Input" );
	}
}

void SupervisedSystemWithoutTrueFlowControl::ProcessAllNetworks()
{
	
}

} //namespace

