#ifndef __SALTOKERNEL__
#define __SALTOKERNEL__

#include "ProcessingKernel.hxx"
#include "AudioManager.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "MIDIHandler.hxx"
#include "MIDIBreathController.hxx"
#include "AudioFileOut.hxx"
#include "Parameters.hxx"
#include "SaltoSynth.hxx"
#include "MelodyTranslator.hxx"
#include "MIDIManager.hxx"
#include "MIDIHandler.hxx"
#include "MIDIInControl.hxx"
#include "Audio.hxx"
#include <string>

namespace SALTO
{

	using CLAM::AudioManager;
	using CLAM::AudioIn;
	using CLAM::AudioOut;
	using CLAM::MIDIManager;
	using CLAM::AudioFileOut;
	using CLAM::Parameters;
	using CLAM::SaltoSynth;
	using CLAM::SaltoSynthConfig;
	using CLAM::MelodyTranslator;
	using CLAM::MelodyTranslatorConfig;
	using CLAM::MIDIManager;
	using CLAM::MIDIHandler;
	using CLAM::Audio;
	using CLAM::MIDIInControl;
	using CLAM::MIDIInConfig;
	using CLAM::MIDIHandlerConfig;

	class Kernel :public CLAM::ProcessingKernel
{
public:

	Kernel();
	virtual ~Kernel();

	void StartProcessing( std::string melodyFile, std::string outputToFile );
	void ProcessingCleanup();
	void ProcessingLoop();

protected:
	bool UserCondition();

	void ConfigureMIDIBasedIO();

	void ConfigureSampleBasedIO( std::string outputFile);

	void ProcessMIDIMessages();

	void RenderSynthesis( Audio& synthBuffer);

private:

	Parameters*       mpParams;
	SaltoSynth*       mpDSP;
	MelodyTranslator  mMelody;
	AudioManager*     mAudioManager;
	AudioIn*          mAudioIn;
	AudioOut*         mAudioOut;
	AudioFileOut      mFileAudioOut;
	MIDIManager       mMIDIManager;
	MIDIHandler       mMIDIHandler;
	MIDIInControl     mKeyboardNote;
	MIDIInControl     mPitchBend;
	BreathController  mBreathController;
	SaltoSynthConfig  mDSPCfg;
	bool              mMayContinue;
};

}

#endif //SALTOKernel.hxx
