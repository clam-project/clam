
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
	CLAM::FlowControl * flow = new CLAM::FlowControl( _oscillatorToFileOut, _frameSize );
	_oscillatorToFileOut.AddFlowControl( flow );
	// create the processing
	_oscillatorToFileOut.AddProcessing("0_oscillator-generator", new CLAM::Oscillator(_generatorCfg) );

	_fileOutCfg.SetFilename( "1_" + _fileOutName );
	_oscillatorToFileOut.AddProcessing("1_file-out", new CLAM::AudioFileOut(_fileOutCfg));

	_oscillatorToFileOut.ConfigurePorts( _frameSize );

	//link them
	_oscillatorToFileOut.ConnectPorts( "0_oscillator-generator.Audio Output", "1_file-out.Input" );

	if (_hasAudioOut)
	{
		_oscillatorToFileOut.AddProcessing( "2_audio-out", new CLAM::AudioOut(_audioOutCfg) );

		_oscillatorToFileOut.ConfigurePorts( _frameSize );
//		_oscillatorToFileOut.GetProcessing("2_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);	

		_oscillatorToFileOut.ConnectPorts( "0_oscillator-generator.Audio Output", "2_audio-out.Input" );
	}
	
	_oscillatorToFileOut.ConfigureNodes(_frameSize);

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut()
{
	CLAM::FlowControl * flow = new CLAM::FlowControl( _fileInFileOut, _frameSize );
	_fileInFileOut.AddFlowControl( flow );

	_fileInFileOut.AddProcessing("0_file-in", new CLAM::AudioFileIn(_fileInCfg));

	_fileOutCfg.SetFilename( "2_" + _fileOutName);
	_fileInFileOut.AddProcessing("1_file-out", new CLAM::AudioFileOut(_fileOutCfg));

	_fileInFileOut.ConfigurePorts( _frameSize );

	_fileInFileOut.ConnectPorts( "0_file-in.Output", "1_file-out.Input" );
	
	if (_hasAudioOut)
	{
		_fileInFileOut.AddProcessing( "2_audio-out", new CLAM::AudioOut(_audioOutCfg) );
		
		_fileInFileOut.ConfigurePorts( _frameSize );
//		_fileInFileOut.GetProcessing("2_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);	
		
		_fileInFileOut.ConnectPorts( "0_file-in.Output", "2_audio-out.Input" );
	}

	_fileInFileOut.ConfigureNodes(_frameSize);

}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn()
{	
	CLAM::FlowControl * flow = new CLAM::FlowControl( _modulatedFileIn, _frameSize );
	_modulatedFileIn.AddFlowControl( flow );

	_modulatedFileIn.AddProcessing("0_file-in", new CLAM::AudioFileIn(_fileInCfg));
	_modulatedFileIn.AddProcessing("1_oscillator-modulator", new CLAM::Oscillator(_modulatorCfg) );
	_modulatedFileIn.AddProcessing("2_multiplier", new CLAM::AudioMultiplier );

	_fileOutCfg.SetFilename( "3_" + _fileOutName );
	_modulatedFileIn.AddProcessing("3_file-out", new CLAM::AudioFileOut(_fileOutCfg));

	_modulatedFileIn.ConfigurePorts( _frameSize );

	_modulatedFileIn.ConnectPorts( "0_file-in.Output", "2_multiplier.First Audio Input" );
	_modulatedFileIn.ConnectPorts( "1_oscillator-modulator.Audio Output", "2_multiplier.Second Audio Input" );
	_modulatedFileIn.ConnectPorts( "2_multiplier.Audio Output", "3_file-out.Input" );	

	if (_hasAudioOut)
	{
		_modulatedFileIn.AddProcessing( "4_audio-out", new CLAM::AudioOut(_audioOutCfg) );
		_modulatedFileIn.ConfigurePorts( _frameSize );
//		_modulatedFileIn.GetProcessing("4_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);

		_modulatedFileIn.ConnectPorts( "2_multiplier.Audio Output", "4_audio-out.Input" );
	}
	_modulatedFileIn.ConfigureNodes(_frameSize);

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedOscillator()
{
	CLAM::FlowControl * flow = new CLAM::FlowControl( _modulatedOscillator, _frameSize );
	_modulatedOscillator.AddFlowControl( flow );

	_modulatedOscillator.AddProcessing( "0_oscillator-generator", new CLAM::Oscillator(_generatorCfg) );
	_modulatedOscillator.AddProcessing( "1_oscillator-modulator", new CLAM::Oscillator(_modulatorCfg) );
	_modulatedOscillator.AddProcessing( "2_multiplier", new CLAM::AudioMultiplier );
	
	_fileOutCfg.SetFilename( "4_" + _fileOutName );
	_modulatedOscillator.AddProcessing("3_file-out", new CLAM::AudioFileOut(_fileOutCfg));

	_modulatedOscillator.ConfigurePorts( _frameSize );

	_modulatedOscillator.ConnectPorts( "0_oscillator-generator.Audio Output", "2_multiplier.First Audio Input" );
	_modulatedOscillator.ConnectPorts( "1_oscillator-modulator.Audio Output", "2_multiplier.Second Audio Input" );
	_modulatedOscillator.ConnectPorts( "2_multiplier.Audio Output", "3_file-out.Input" );	

	if (_hasAudioOut)
	{
		_modulatedOscillator.AddProcessing( "4_audio-out", new CLAM::AudioOut(_audioOutCfg) );
		_modulatedOscillator.ConfigurePorts( _frameSize );
//		_modulatedOscillator.GetProcessing("4_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);
		_modulatedOscillator.ConnectPorts( "2_multiplier.Audio Output", "4_audio-out.Input" );
	}
	_modulatedOscillator.ConfigureNodes(_frameSize);
}

void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileInPlusFileIn()
{	
	CLAM::FlowControl * flow = new CLAM::FlowControl(_modulatedFileInPlusFileIn, _frameSize );
	_modulatedFileInPlusFileIn.AddFlowControl( flow );

	_modulatedFileInPlusFileIn.AddProcessing("0_file-in", new CLAM::AudioFileIn(_fileInCfg));
	_modulatedFileInPlusFileIn.AddProcessing("1_oscillator-modulator", new CLAM::Oscillator(_modulatorCfg) );
	_modulatedFileInPlusFileIn.AddProcessing("2_multiplier", new CLAM::AudioMultiplier );
	_modulatedFileInPlusFileIn.AddProcessing( "3_mixer", new CLAM::AudioMixer<2>(_mixerCfg) );
	
	_fileOutCfg.SetFilename( "5_"+_fileOutName );
	_modulatedFileInPlusFileIn.AddProcessing("4_file-out", new CLAM::AudioFileOut(_fileOutCfg));
	
	_modulatedFileInPlusFileIn.ConfigurePorts( _frameSize );

	_modulatedFileInPlusFileIn.ConnectPorts( "0_file-in.Output", "2_multiplier.First Audio Input" );
	_modulatedFileInPlusFileIn.ConnectPorts( "1_oscillator-modulator.Audio Output", "2_multiplier.Second Audio Input" );
	_modulatedFileInPlusFileIn.ConnectPorts( "0_file-in.Output", "3_mixer.Input Audio_1" );
	_modulatedFileInPlusFileIn.ConnectPorts( "2_multiplier.Audio Output" , "3_mixer.Input Audio_0" );
	_modulatedFileInPlusFileIn.ConnectPorts( "3_mixer.Output Audio", "4_file-out.Input" );


	if (_hasAudioOut)
	{
		_modulatedFileInPlusFileIn.AddProcessing( "5_audio-out", new CLAM::AudioOut(_audioOutCfg) );
		_modulatedFileInPlusFileIn.ConfigurePorts( _frameSize );
//		_modulatedFileInPlusFileIn.GetProcessing("5_audio-out").GetInPorts().Get("Input").SetParams(_frameSize);
		_modulatedFileInPlusFileIn.ConnectPorts( "3_mixer.Output Audio", "5_audio-out.Input" );
	}
	_modulatedFileInPlusFileIn.ConfigureNodes(_frameSize);
}

void SupervisedSystemWithoutTrueFlowControl::ProcessAllNetworks()
{

	NetworkList::iterator it;
	for ( it=_networks.begin(); it != _networks.end(); it++ )
	{
		(*it)->Start();
		for (int i=0; i<_maxFramesToProcess; i++)
			(*it)->DoProcessings();

		std::cout << (*it)->GetName() << " network processed.\n";
		(*it)->Stop();
	}
}

} //namespace

