#include "NonSupervisedSystem.hxx"

namespace FlowControlExample
{

System::System( std::string fileOut , int frameSize , int iterations) : 
	_fileOutName(fileOut), 
	_frameSize(frameSize), 
	_iterations(iterations)
{
	ConfigureProcessings();
	ConfigureData();

	StartProcessings();
}

void System::ConfigureProcessings()
{
	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(1.0);

	_oscillator.Configure(oscilCfg);

	CLAM::AudioFileConfig fileOutCfg;
	fileOutCfg.SetFilename( _fileOutName );
	fileOutCfg.SetChannels(1);
	fileOutCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileOutCfg.SetFrameSize( _frameSize );

	_fileOut.Configure( fileOutCfg );
}

void System::ConfigureData()
{
	_oscillatorOut.SetSize(_frameSize);
}

void System::StartProcessings()
{
	_oscillator.Start();
	_fileOut.Start();
}

void System::DoProcessings()
{
	for (int i=0;i<_iterations;i++)
	{
		_oscillator.Do(_oscillatorOut);
		_fileOut.Do(_oscillatorOut);
	}
}


}