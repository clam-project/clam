
#include "SupervisedSystemWithoutTrueFlowControl.hxx"


#include "AudioMultiplier.hxx"
#include "FlowControl.hxx"

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
	_fileOutName(fileOut),
	// networks
	_oscillatorToFileOut("oscillator to file-out"),
	_fileInFileOut("file in file-out"),
	_modulatedFileIn("modulated file-in"),
	_modulatedOscillator("modulated oscillator"),
	_modulatedFileInPlusFileIn("modulated file-in plus file-in")
{
	InitConfigurations();
	ConfigureNetworks();
}

void SupervisedSystemWithoutTrueFlowControl::InitConfigurations()
{
	_generatorCfg.SetFrequency(440.0);
	_generatorCfg.SetAmplitude(0.5);

	_modulatorCfg.SetFrequency(220.0);
	_modulatorCfg.SetAmplitude(0.5);

	_fileOutCfg.SetChannels(1);
	_fileOutCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	_fileOutCfg.SetFrameSize( _frameSize );
	_fileOutCfg.SetKeepFrameSizes(true);

	_fileInCfg.SetChannels(1);
	_fileInCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	_fileInCfg.SetFrameSize( _frameSize );
	_fileInCfg.SetFilename( _fileInName );
	_fileInCfg.SetKeepFrameSizes(true);

	_audioOutCfg.SetFrameSize(_frameSize);

	_mixerCfg.SetFrameSize(_frameSize);
}

void SupervisedSystemWithoutTrueFlowControl::ConfigureNetworks()
{
	ConfigureFileInFileOut();
	ConfigureOscillatorToFileOut();
	ConfigureModulatedFileIn();
	ConfigureModulatedOscillator();
	ConfigureModulatedFileInPlusFileIn();

	_networks.push_back(&_fileInFileOut);
	_networks.push_back(&_oscillatorToFileOut);
	_networks.push_back(&_modulatedFileIn);
	_networks.push_back(&_modulatedOscillator);
	_networks.push_back(&_modulatedFileInPlusFileIn);
}

void SupervisedSystemWithoutTrueFlowControl::ConfigureOscillatorToFileOut()
{	
	_oscillatorToFileOut.AddFlowControl( new CLAM::FlowControl( _frameSize ));
	// create the processing
	_oscillatorToFileOut.AddProcessing("oscillator-generator", new CLAM::Oscillator(_generatorCfg) );

	_fileOutCfg.SetFilename( "1_" + _fileOutName );
	_oscillatorToFileOut.AddProcessing("file-out", new CLAM::AudioFileOut(_fileOutCfg));

	_oscillatorToFileOut.ConfigurePorts( _frameSize );

	//link them
	_oscillatorToFileOut.ConnectPorts( "oscillator-generator.Audio Output", "file-out.Input" );

	if (_hasAudioOut)
	{
		_oscillatorToFileOut.AddProcessing( "audio-out", new CLAM::AudioOut(_audioOutCfg) );

		_oscillatorToFileOut.ConfigurePorts( _frameSize );
//		_oscillatorToFileOut.GetProcessing("2_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);	

		_oscillatorToFileOut.ConnectPorts( "oscillator-generator.Audio Output", "audio-out.Input" );
	}
	
	_oscillatorToFileOut.ConfigureNodes(_frameSize);

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut()
{
	_fileInFileOut.AddFlowControl( new CLAM::FlowControl( _frameSize ));

	_fileInFileOut.AddProcessing("file-in", new CLAM::AudioFileIn(_fileInCfg));

	_fileOutCfg.SetFilename( "2_" + _fileOutName);
	_fileInFileOut.AddProcessing("file-out", new CLAM::AudioFileOut(_fileOutCfg));

	_fileInFileOut.ConfigurePorts( _frameSize );

	_fileInFileOut.ConnectPorts( "file-in.Output", "file-out.Input" );
	
	if (_hasAudioOut)
	{
		_fileInFileOut.AddProcessing( "audio-out", new CLAM::AudioOut(_audioOutCfg) );
		
		_fileInFileOut.ConfigurePorts( _frameSize );
//		_fileInFileOut.GetProcessing("2_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);	
		
		_fileInFileOut.ConnectPorts( "file-in.Output", "audio-out.Input" );
	}

	_fileInFileOut.ConfigureNodes(_frameSize);

}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn()
{	
	_modulatedFileIn.AddFlowControl( new CLAM::FlowControl( _frameSize ));

	_modulatedFileIn.AddProcessing("file-in", new CLAM::AudioFileIn(_fileInCfg));
	_modulatedFileIn.AddProcessing("oscillator-modulator", new CLAM::Oscillator(_modulatorCfg) );
	_modulatedFileIn.AddProcessing("multiplier", new CLAM::AudioMultiplier );

	_fileOutCfg.SetFilename( "3_" + _fileOutName );
	_modulatedFileIn.AddProcessing("file-out", new CLAM::AudioFileOut(_fileOutCfg));

	_modulatedFileIn.ConfigurePorts( _frameSize );

	_modulatedFileIn.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
	_modulatedFileIn.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_modulatedFileIn.ConnectPorts( "multiplier.Audio Output", "file-out.Input" );	

	if (_hasAudioOut)
	{
		_modulatedFileIn.AddProcessing( "audio-out", new CLAM::AudioOut(_audioOutCfg) );
		_modulatedFileIn.ConfigurePorts( _frameSize );
//		_modulatedFileIn.GetProcessing("4_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);

		_modulatedFileIn.ConnectPorts( "multiplier.Audio Output", "audio-out.Input" );
	}
	_modulatedFileIn.ConfigureNodes(_frameSize);

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedOscillator()
{
	_modulatedOscillator.AddFlowControl( new CLAM::FlowControl( _frameSize ));

	_modulatedOscillator.AddProcessing( "oscillator-generator", new CLAM::Oscillator(_generatorCfg) );
	_modulatedOscillator.AddProcessing( "oscillator-modulator", new CLAM::Oscillator(_modulatorCfg) );
	_modulatedOscillator.AddProcessing( "multiplier", new CLAM::AudioMultiplier );
	
	_fileOutCfg.SetFilename( "4_" + _fileOutName );
	_modulatedOscillator.AddProcessing("file-out", new CLAM::AudioFileOut(_fileOutCfg));

	_modulatedOscillator.ConfigurePorts( _frameSize );

	_modulatedOscillator.ConnectPorts( "oscillator-generator.Audio Output", "multiplier.First Audio Input" );
	_modulatedOscillator.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_modulatedOscillator.ConnectPorts( "multiplier.Audio Output", "file-out.Input" );	

	if (_hasAudioOut)
	{
		_modulatedOscillator.AddProcessing( "audio-out", new CLAM::AudioOut(_audioOutCfg) );
		_modulatedOscillator.ConfigurePorts( _frameSize );
//		_modulatedOscillator.GetProcessing("4_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);
		_modulatedOscillator.ConnectPorts( "multiplier.Audio Output", "audio-out.Input" );
	}
	_modulatedOscillator.ConfigureNodes(_frameSize);
}

void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileInPlusFileIn()
{	
	_modulatedFileInPlusFileIn.AddFlowControl( new CLAM::FlowControl( _frameSize ));

	_modulatedFileInPlusFileIn.AddProcessing("file-in", new CLAM::AudioFileIn(_fileInCfg));
	_modulatedFileInPlusFileIn.AddProcessing("oscillator-modulator", new CLAM::Oscillator(_modulatorCfg) );
	_modulatedFileInPlusFileIn.AddProcessing("multiplier", new CLAM::AudioMultiplier );
	_modulatedFileInPlusFileIn.AddProcessing( "mixer", new CLAM::AudioMixer<2>(_mixerCfg) );
	
	_fileOutCfg.SetFilename( "5_"+_fileOutName );
	_modulatedFileInPlusFileIn.AddProcessing("file-out", new CLAM::AudioFileOut(_fileOutCfg));
	
	_modulatedFileInPlusFileIn.ConfigurePorts( _frameSize );

	_modulatedFileInPlusFileIn.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
	_modulatedFileInPlusFileIn.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_modulatedFileInPlusFileIn.ConnectPorts( "file-in.Output", "mixer.Input Audio_1" );
	_modulatedFileInPlusFileIn.ConnectPorts( "multiplier.Audio Output" , "mixer.Input Audio_0" );
	_modulatedFileInPlusFileIn.ConnectPorts( "mixer.Output Audio", "file-out.Input" );


	if (_hasAudioOut)
	{
		_modulatedFileInPlusFileIn.AddProcessing( "audio-out", new CLAM::AudioOut(_audioOutCfg) );
		_modulatedFileInPlusFileIn.ConfigurePorts( _frameSize );
//		_modulatedFileInPlusFileIn.GetProcessing("5_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);
		_modulatedFileInPlusFileIn.ConnectPorts( "mixer.Output Audio", "audio-out.Input" );
	}
	_modulatedFileInPlusFileIn.ConfigureNodes(_frameSize);
}

void SupervisedSystemWithoutTrueFlowControl::ProcessAllNetworks()
{

	NetworkList::iterator it;
	for ( it=_networks.begin(); it != _networks.end(); it++ )
	{
		(*it)->Start();
			(*it)->DoProcessings();

		std::cout << (*it)->GetName() << " network processed.\n";
		(*it)->Stop();
	}
}


void SupervisedSystemWithoutTrueFlowControl::ProcessAllNetworkTopologies()
{
	NetworkConfigurationMethods::iterator currentConfigMethod;
	for ( currentConfigMethod=_configurations.begin(); 
		  currentConfigMethod != _configurations.end(); 
		  currentConfigMethod++ )
	{
		_network.DisconnectAllPorts();
		
		currentConfigMethod->Configure(_network);

		_network.Start();

		for (int i=0; i<_maxFramesToProcess; i++)
			_network.DoProcessings();

		std::cout << _network.GetName() << " network processed.\n";
		_network.Stop();
	}
}


} //namespace

