#include "NonSupervisedSystemWithPorts.hxx"

#include <iostream>

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

	// oscillators
	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);

	_oscillator.Configure(oscilCfg);

	oscilCfg.SetFrequency(220.0);
	_modulator.Configure(oscilCfg);

	// Audio File In & Out
	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

	_fileOut.Configure( fileCfg );

	fileCfg.SetFilename(_fileInName);

	_fileIn.Configure (fileCfg);

}

void System::ConfigureData()
{
	_oscillatorData.SetSize(_frameSize);
	_fileInData.SetSize(_frameSize);
	_modulatorData.SetSize(_frameSize);
	_multiplierData.SetSize(_frameSize);
	_adderData.SetSize(_frameSize);
}

void System::StartProcessings()
{
	try{
	_oscillator.Start();
	_fileOut.Start();
	_fileIn.Start();
	_modulator.Start();
	_multiplier.Start();
	_adder.Start();
	}
	catch (CLAM::ErrProcessingObj& e)
	{
		CLAM_ASSERT(false, e.what());
	}
}

bool System::OscillatorToFileOut() 
{
	_oscillator.Do();
	_fileOut.Do();
	return false;
}

bool System::ModulatedFileIn()
{
	_fileIn.Do();
	_modulator.Do();
	_multiplier.Do();
	_fileOut.Do();
	return false;
}

bool System::ModulatedOscillator()
{
	_oscillator.Do();
	_modulator.Do();
	_multiplier.Do();
	_fileOut.Do();
	return false;
}

bool System::ModulatedFileInPlusFileIn()
{
	_fileIn.Do();
	_modulator.Do();
	_multiplier.Do();
	_oscillator.Do();
	_adder.Do();
	_fileOut.Do();
	return false;
}

bool System::FileInFileOut()
{
	_fileIn.Do();
	_fileOut.Do();
	return false;
}

bool System::StablishConnectionsOscillatorToFileOut()
{
	std::cout << "attaching oscillator to file out" << std::endl;

	_oscillator.mOutput.Attach(_oscillatorData);
	_fileOut.Input.Attach(_oscillatorData);

	return true;
}

bool System::StablishConnectionsModulatedFileIn()
{
	return true;
}

bool System::StablishConnectionsModulatedOscillator()
{
	return true;
}

bool System::StablishConnectionsModulatedFileInPlusFileIn()
{
	return true;
}

bool System::StablishConnectionsFileInFileOut()
{
	return true;
}

void System::StablishConnections( StablishConnectionsMethod connect)
{
	(this->*connect)();
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
	std::cout << "oscillatortofileout" << std::endl;
	DoProcessings( &System::OscillatorToFileOut );

	std::cout << "modulatedoscillator" << std::endl;
	DoProcessings( &System::ModulatedOscillator );

	std::cout << "fileinfileout" << std::endl;
	DoProcessings( &System::FileInFileOut );

	_fileIn.Stop();
	_fileIn.Start();
	std::cout << "modulatedfilein" << std::endl;
	DoProcessings( &System::ModulatedFileIn );

	_fileIn.Stop();
	_fileIn.Start();
	std::cout << "modulatedfileinplusfilein" << std::endl;
	DoProcessings( &System::ModulatedFileInPlusFileIn);
}

} // namespace
