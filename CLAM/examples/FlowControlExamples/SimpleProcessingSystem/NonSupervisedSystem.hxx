#ifndef _NonSupervisedSystem_hxx_
#define _NonSupervisedSystem_hxx_

#include "Oscillator.hxx"
#include "AudioFileOut.hxx"

#include "Audio.hxx"
#include <list>

namespace FlowControlExample
{

class System
{
	// Iteration method type
	typedef bool (System::*IterationMethod)(void);

public:
	virtual ~System() {};
	System( std::string fileOut , int frameSize, int iterations);
	
	void DoProcessings( IterationMethod );
	
	void ProcessAllIterations();
	
private:
	void StartProcessings();
	void ConfigureProcessings();
	void ConfigureData();
	

	// Iteration methods

	bool OscillatorToFileOut();
	// processings
	CLAM::Oscillator _oscillator;
	CLAM::AudioFileOut _fileOut;

	// processing data
	CLAM::Audio _oscillatorOut;

	//other system parameters
	std::string _fileOutName;
	int _frameSize;
	int _iterations;

};

} // namespace

#endif
