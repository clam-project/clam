
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

	//_networks.push_back(&_fileInFileOut);
	_networks.push_back(&_oscillatorToFileOut);
	//_networks.push_back(&_modulatedFileIn);
	//_networks.push_back(&_modulatedOscillator);
	//_networks.push_back(&_modulatedFileInPlusFileIn);
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

	_oscillatorToFileOut.AddProcessing("0_oscillator-generator", new CLAM::Oscillator(oscilCfg) );

	_oscillatorToFileOut.AddProcessing("1_file-out", new CLAM::AudioFileOut(fileCfg));

	//link them
	_oscillatorToFileOut.ConnectPorts( "0_oscillator-generator.Audio Output", "1_file-out.Input" );

	if (_hasAudioOut)
	{

		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);

		_oscillatorToFileOut.AddProcessing( "2_audio-out", new CLAM::AudioOut(audioCfg) );
		_oscillatorToFileOut.ConnectPorts( "0_oscillator-generator.Audio Output", "2_audio-out.Input" );
	}

//	_oscillatorToFileOut.GetProcessing("0_oscillator-generator").GetOutPorts().Get("Audio Output").SetParams(_frameSize);
//	_oscillatorToFileOut.GetProcessing("1_file-out").GetInPorts().Get("Input").SetParams(_frameSize);

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut()
{

	CLAM::AudioFileConfig fileCfg;
	
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);
	fileCfg.SetFilename( _fileInName );

	_fileInFileOut.AddProcessing("0_file-in", new CLAM::AudioFileIn(fileCfg));

	fileCfg.SetFilename( _fileOutName );

	_fileInFileOut.AddProcessing("1_file-out", new CLAM::AudioFileOut(fileCfg));


	_fileInFileOut.ConnectPorts( "0_file-in.Output", "1_file-out.Input" );
	
	if (_hasAudioOut)
	{
		
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);
		
		_fileInFileOut.AddProcessing( "2_audio-out", new CLAM::AudioOut(audioCfg) );
		_fileInFileOut.ConnectPorts( "0_file-in.Output", "2_audio-out.Input" );
	}

	

}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn()
{
	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetKeepFrameSizes(true);
	fileCfg.SetFrameSize( _frameSize );

	fileCfg.SetFilename( _fileInName );
	_modulatedFileIn.AddProcessing("file-in", new CLAM::AudioFileIn(fileCfg));

	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);
	
	_modulatedFileIn.AddProcessing("oscillator-modulator", new CLAM::Oscillator(oscilCfg) );

	_modulatedFileIn.AddProcessing("multiplier", new CLAM::AudioMultiplier );

	fileCfg.SetFilename( _fileOutName );
	_modulatedFileIn.AddProcessing("file-out", new CLAM::AudioFileOut(fileCfg));

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
	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);
	
	_modulatedOscillator.AddProcessing( "oscillator-generator", new CLAM::Oscillator(oscilCfg) );

	oscilCfg.SetFrequency(220.0);
	_modulatedOscillator.AddProcessing( "oscillator-modulator", new CLAM::Oscillator(oscilCfg) );

	_modulatedOscillator.AddProcessing( "multiplier", new CLAM::AudioMultiplier );

		CLAM::AudioFileConfig fileCfg;
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

	fileCfg.SetFilename( _fileOutName );
	_modulatedOscillator.AddProcessing("file-out", new CLAM::AudioFileOut(fileCfg));

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
	
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

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
	
	fileCfg.SetFilename( _fileOutName );
	_modulatedFileInPlusFileIn.AddProcessing("file-out", new CLAM::AudioFileOut(fileCfg));
	
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
/*
	NetworkList::iterator it;
	for ( it=_networks.begin(); it != _networks.end(); it++ )
	{
		(*it)->ConfigureNodes(_frameSize);
		(*it)->Start();
		for (int i=0; i<_maxFramesToProcess; i++)
			(*it)->DoProcessings();
		(*it)->Stop();
	}
*/

	_oscillatorToFileOut.Start();
	_oscillatorToFileOut.ConfigureNodes(_frameSize);


	_oscillatorToFileOut.GetProcessing("0_oscillator-generator").GetOutPorts().Get("Audio Output").SetParams(_frameSize);
	_oscillatorToFileOut.GetProcessing("1_file-out").GetInPorts().Get("Input").SetParams(_frameSize);


	for (int i=0; i<_maxFramesToProcess; i++)
	{
		_oscillatorToFileOut.GetProcessing("0_oscillator-generator").Do();
		_oscillatorToFileOut.GetProcessing("1_file-out").Do();
	}
	_oscillatorToFileOut.Stop();
	
}

} //namespace

