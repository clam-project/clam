#include "NonSupervisedSystemWithPorts.hxx"

#include <iostream>

namespace FlowControlExample
{

bool NetworkConfiguration::trace = true;

void NetworkConfiguration::ConnectAndDo()
{
	if (trace) std::cout << "Processing: " << GetName();
	Start();
	Connect(); 
	if (trace) std::cout << "  Connected.  ";
	for (int i=0; i<_system->GetMaxFramesToProcess(); i++) {
		if ( Do() ) break;
	}
	if (trace) std::cout <<"Done.\n";
	Stop();
}


SystemWithPorts::SystemWithPorts( std::string fileIn, std::string fileOut , int frameSize , int nFrames) : 
	_fileInName(fileIn),
	_fileOutName(fileOut), 
	_frameSize(frameSize), 
	_maxFramesToProcess(nFrames)
{
	AddNetworkConfiguration( new OscillatorToFileOut(this) );
	AddNetworkConfiguration( new FileInFileOut(this) );
	AddNetworkConfiguration( new ModulatedFileIn(this) );
	AddNetworkConfiguration( new ModulatedOscillator(this) );
	AddNetworkConfiguration( new ModulatedFileInPlusFileIn(this) );

	ConfigureProcessings();
	ConfigureData();
	StartProcessings();
}
SystemWithPorts::~SystemWithPorts()
{
	RemoveAllNetworkConfigurations();
}

void SystemWithPorts::ConfigureProcessings()
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

void SystemWithPorts::ConfigureData()
{
	_oscillatorData.SetSize(_frameSize);
	_fileInData.SetSize(_frameSize);
	_modulatorData.SetSize(_frameSize);
	_multiplierData.SetSize(_frameSize);
	_adderData.SetSize(_frameSize);
}

void SystemWithPorts::StartProcessings()
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

void SystemWithPorts::OscillatorToFileOut::Connect()
{
	System()._oscillator.mOutput.Attach( System()._oscillatorData );
	System()._fileOut.Input.Attach( System()._oscillatorData );
}
bool SystemWithPorts::OscillatorToFileOut::Do() 
{
	System()._oscillator.Do();
	System()._fileOut.Do();
	return false;
}

void SystemWithPorts::ModulatedFileIn::Connect()
{
	System()._fileIn.mOutput.Attach( System()._fileInData );
	System()._modulator.mOutput.Attach( System()._modulatorData );
	System()._multiplier.mFirstInput.Attach( System()._fileInData );
	System()._multiplier.mSecondInput.Attach( System()._modulatorData );
	System()._multiplier.mOutput.Attach( System()._multiplierData );
	System()._fileOut.Input.Attach( System()._multiplierData );
}
bool SystemWithPorts::ModulatedFileIn::Do()
{
	System()._fileIn.Do();
	System()._modulator.Do();
	System()._multiplier.Do();
	System()._fileOut.Do();
	return false;
}
void SystemWithPorts::ModulatedFileIn::Stop()
{
	System()._fileIn.Stop();
	System()._fileIn.Start();
}

void SystemWithPorts::ModulatedOscillator::Connect()
{
	System()._oscillator.mOutput.Attach( System()._oscillatorData );
	System()._modulator.mOutput.Attach( System()._modulatorData );
	System()._multiplier.mFirstInput.Attach( System()._oscillatorData );
	System()._multiplier.mSecondInput.Attach( System()._modulatorData );
	System()._multiplier.mOutput.Attach( System()._multiplierData );
	System()._fileOut.Input.Attach( System()._multiplierData );
}
bool SystemWithPorts::ModulatedOscillator::Do()
{
	System()._oscillator.Do();
	System()._modulator.Do();
	System()._multiplier.Do();
	System()._fileOut.Do();
	return false;
}

void SystemWithPorts::FileInFileOut::Connect()
{
	System()._fileIn.mOutput.Attach( System()._fileInData );
	System()._fileOut.Input.Attach( System()._fileInData );
}
bool SystemWithPorts::FileInFileOut::Do()
{
	System()._fileIn.Do();
	System()._fileOut.Do();
	return false;
}
void SystemWithPorts::FileInFileOut::Stop()
{
	System()._fileIn.Stop();
	System()._fileIn.Start();
}

void SystemWithPorts::ModulatedFileInPlusFileIn::Connect()
{
	System()._fileIn.mOutput.Attach( System()._fileInData );
	System()._modulator.mOutput.Attach( System()._modulatorData );
	System()._multiplier.mFirstInput.Attach( System()._fileInData );
	System()._multiplier.mSecondInput.Attach( System()._modulatorData );
	System()._multiplier.mOutput.Attach( System()._multiplierData );
	System()._adder.mFirstInput.Attach( System()._multiplierData );
	System()._adder.mSecondInput.Attach( System()._fileInData );
	System()._adder.mOutput.Attach( System()._adderData );
	System()._fileOut.Input.Attach( System()._adderData );
}
bool SystemWithPorts::ModulatedFileInPlusFileIn::Do()
{
	System()._fileIn.Do();
	System()._modulator.Do();
	System()._multiplier.Do();
	System()._adder.Do();
	System()._fileOut.Do();
	return false;
}
void SystemWithPorts::ModulatedFileInPlusFileIn::Stop() {
	System()._fileIn.Stop();
	System()._fileIn.Start();
}


void SystemWithPorts::AddNetworkConfiguration( NetworkConfiguration* item)
{
	_networks.push_back( item );
}
void SystemWithPorts::RemoveAllNetworkConfigurations()
{
	NetworkConfigurations::iterator it;
	for (it=_networks.begin(); it!=_networks.end(); it++)
		delete (*it);
}
void SystemWithPorts::ProcessAllNetworkConfigurations()
{
	NetworkConfigurations::iterator it;
	for (it=_networks.begin(); it!=_networks.end(); it++) {
		(*it)->ConnectAndDo();
	}
}

} // namespace
