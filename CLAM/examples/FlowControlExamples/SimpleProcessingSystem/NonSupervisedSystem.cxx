#include "NonSupervisedSystem.hxx"

namespace FlowControlExample
{

System::System( std::string fileIn, std::string fileOut , int frameSize , int nFrames) : 
	_fileInName(fileIn),
	_fileOutName(fileOut), 
	_frameSize(frameSize), 
	_maxFramesToProcess(nFrames)
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

	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );

	_fileOut.Configure( fileCfg );

	fileCfg.SetFilename(_fileInName);

	_fileIn.Configure (fileCfg);
}

void System::ConfigureData()
{
	_oscillatorData.SetSize(_frameSize);
	_fileInData.SetSize(_frameSize);
	_modulatorData.SetSize(_frameSize);
	_adderData.SetSize(_frameSize);
}

void System::StartProcessings()
{
	try{
	_oscillator.Start();
	_fileOut.Start();
	_fileIn.Start();
	}
	catch (CLAM::ErrProcessingObj& e)
	{
		CLAM_ASSERT(false, e.what());
	}
}

bool System::OscillatorToFileOut() {
	_oscillator.Do(_oscillatorData);
	_fileOut.Do(_oscillatorData);
	return false;
}

bool System::ModulatedFileIn()
{
	_fileIn.Do(_fileInData);
	_fileOut.Do(_fileInData);
	return false;
}

void System::DoProcessings( IterationMethod iterationDo )
{
	for (int i=0; i<_maxFramesToProcess; i++)
	{
		if ( (this->*iterationDo)() ) 
		{
			break;
		}
	}
}

void System::ProcessAllIterations()
{
	DoProcessings( &System::OscillatorToFileOut );
	DoProcessings( &System::ModulatedFileIn );
}

} // namespace
