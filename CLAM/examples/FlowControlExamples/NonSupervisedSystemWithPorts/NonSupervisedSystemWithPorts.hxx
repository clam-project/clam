#ifndef _NonSupervisedSystem_hxx_
#define _NonSupervisedSystem_hxx_

#include "Oscillator.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"
#include "AudioAdder.hxx"
#include "AudioMultiplier.hxx"

#include "Audio.hxx"
#include <list>
#include <string>


namespace FlowControlExample
{

// forward decl
class SystemWithPorts;

class NetworkConfiguration
{
public: 
	NetworkConfiguration( SystemWithPorts* sys, std::string name) : 
	  _system(sys),
	  _name(name)
	{}

	// returns true if has finished the processing (i.e. end-of-file)
	virtual bool Do() = 0;
	virtual void Connect()  =0;
	virtual void Start() {};
	virtual void Stop() {};
	const std::string& GetName() const { return _name; }
	
	void ConnectAndDo();
	virtual ~NetworkConfiguration() {};

protected:
	/// to be used in template methods definitions Do() and Connect()
	SystemWithPorts& System() { return *_system; }

private:
	SystemWithPorts* _system;
	const std::string _name;
	static bool trace;
};

//----------------------------------------------------------------------
class SystemWithPorts
{

	typedef std::list<NetworkConfiguration*> NetworkConfigurations;

public:
	virtual ~SystemWithPorts();
	SystemWithPorts( std::string fileIn, std::string fileOut , int frameSize, int maxFramesToProcess);
	
	void ProcessAllNetworkConfigurations();
	void RemoveAllNetworkConfigurations();
	void AddNetworkConfiguration( NetworkConfiguration* );

	int GetMaxFramesToProcess() const {
		return _maxFramesToProcess;
	}
	// NetworkConfiguration classes:
	
	class OscillatorToFileOut : public NetworkConfiguration {
	public:
		OscillatorToFileOut( SystemWithPorts *parent ) : 
		  NetworkConfiguration(parent, "OscillatorToFileOut") {}
		bool Do();
		void Connect();
	};
	
	class FileInFileOut : public NetworkConfiguration {
	public:
		FileInFileOut( SystemWithPorts *parent ) : 
		  NetworkConfiguration(parent, "FileInFileOut") {}
		bool Do();
		void Connect();
		void Stop();
	};
	
	class ModulatedFileIn : public NetworkConfiguration {
	public:
		ModulatedFileIn( SystemWithPorts *parent ) :  
		  NetworkConfiguration(parent, "ModulatedFileIn") {}
		bool Do();
		void Connect();
		void Stop();
	};

	class ModulatedOscillator : public NetworkConfiguration {
	public:
		ModulatedOscillator( SystemWithPorts *parent ) :  
		  NetworkConfiguration(parent, "ModulatedOscillator") {}
		bool Do();
		void Connect();
	};

	class ModulatedFileInPlusFileIn : public NetworkConfiguration {
	public:
		ModulatedFileInPlusFileIn( SystemWithPorts *parent ) : 
		  NetworkConfiguration(parent, "ModulatedFileInPlusFileIn") {}
		bool Do();
		void Connect();
		void Stop();
	};
	

private:
	void StartProcessings();
	void ConfigureProcessings();
	void ConfigureData();

	// processings
	CLAM::Oscillator _oscillator;
	CLAM::Oscillator _modulator;
	CLAM::AudioFileIn _fileIn;
	CLAM::AudioFileOut _fileOut;
	CLAM::AudioAdder _adder;
	CLAM::AudioMultiplier _multiplier;

	// processing data
	CLAM::Audio _oscillatorData;
	CLAM::Audio _fileInData;
	CLAM::Audio _modulatorData;
	CLAM::Audio _adderData;
	CLAM::Audio _multiplierData;

	//other system parameters
	std::string _fileInName;
	std::string _fileOutName;
	int _frameSize;
	int _maxFramesToProcess;

	//system infrastructure
	NetworkConfigurations _networks;

};

} // namespace

#endif
