#include "SALTOKernel.hxx"
#include "SaltoSynth.hxx"
#include "Parameters.hxx"
#include "CSaltoDataManagment.hxx"
#include "DataTypes.hxx"
#include "Assert.hxx"
#include "ErrDynamicType.hxx"

namespace SALTO
{
	using CLAM::Audio;
	using CLAM::TTime;
	using CLAM::MIDI;
	using CLAM::AudioFileConfig;
	using CLAM::EAudioFileType;
	using CLAM::AudioIOConfig;

	Kernel::Kernel()
	{
		mpParams = Parameters::GetInstance();
		// SDIF database loading...
		mDSPCfg.SetParams( *mpParams );
		mpDSP = new SaltoSynth(mDSPCfg);
	}

	Kernel::~Kernel()
	{
	}

	bool Kernel::UserCondition()
	{
		return mMayContinue;
	}

	void Kernel::StartProcessing( std::string melodyFile, std::string outputToFile )
	{
		CSaltoDataManagment::InitSaltoDB( mpParams );

		MIDIHandlerConfig MIDIHandlerCfg;
		MIDIHandlerCfg.SetTranspose( mpParams->GetTranspose() );
		MIDIHandlerCfg.SetPitchModRange( mDSPCfg.GetPitchModRange() );
		mMIDIHandler.Configure( MIDIHandlerCfg );

		mMIDIHandler.LinkOutWithInControl( 0, mpDSP, 0 ); 

		MelodyTranslatorConfig melodyCfg;
		melodyCfg.SetFileName( melodyFile );
/*		if ( melodyFile != "None" )
			mpParams->SetUseMelody(true);
*/
		mpParams->SetUseBreathController(true);
		mMelody.Configure( melodyCfg );
		mMelody.LinkOutWithInControl( 0, &mMIDIHandler, 0);
		mMelody.LinkOutWithInControl( 1, &mMIDIHandler, 1);
		
		mpDSP->ResetEventSample();
		if ( outputToFile != "None" )
			mpParams->SetWriteToFile(true);
		ConfigureSampleBasedIO(  outputToFile );
		ConfigureMIDIBasedIO();

	}

	void Kernel::ProcessingLoop()
	{
		mMayContinue = true;
		Audio* synthbuffer = NULL;

		mpDSP->Start();
		try
		{
			mMelody.Start();
		}
		catch( CLAM::ErrDynamicType& e )
		{
			e.Print();
			exit(-1);
		}
		mFileAudioOut.Start();
		mMIDIManager.Start();
		mAudioManager->Start();
		mMIDIHandler.Start();

		while ( LoopCondition() )
			{
				ProcessMIDIMessages();
				
				mMayContinue = mpDSP->Do( synthbuffer );
				
				CLAM_DEBUG_ASSERT( synthbuffer != NULL, "Whooops! Synthesis buffer was void!" );
							
				RenderSynthesis( *synthbuffer );

			}


	}

	void Kernel::ProcessingCleanup()
	{
		mpDSP->Stop();
		mMelody.Stop();
		mFileAudioOut.Stop();
		mMIDIManager.Stop();
		mMIDIHandler.Stop();
	}

	void Kernel::ProcessMIDIMessages()
	{
		if (mpParams->GetUseMelody())
			{
				TTime currentTime = mpDSP->GetEventSample()/mDSPCfg.GetSampleRate();
				if (!mMelody.Do( mpParams->GetPlay(), currentTime ))
					{
						mMIDIHandler.Do( *mpParams );
						mpParams->SetUseMelody( false );
						mpDSP->ResetEventSample();
					}
				else
					mMIDIHandler.Do( *mpParams );
			}
			
		if ( mpParams->GetUseMidiKeyboard() || mpParams->GetUseBreathController() )
			{
				mMIDIManager.Check();
				mMIDIHandler.Do( *mpParams );
			}		

		// End of SALTO MIDI Messages Processing
		
	}

	void Kernel::ConfigureMIDIBasedIO()
	{
		MIDIInConfig inNoteCfg;

		inNoteCfg.SetName("in");
		inNoteCfg.SetDevice("default:default");
		inNoteCfg.SetChannelMask( 
									MIDI::ChannelMask(1) |
									MIDI::ChannelMask(2)
								);

		inNoteCfg.SetMessageMask(
			MIDI::MessageMask(MIDI::eNoteOn)|
			MIDI::MessageMask(MIDI::eNoteOff)
		);

		mKeyboardNote.Configure( inNoteCfg );

		MIDIInConfig inPitchBendCfg;
		
		inPitchBendCfg.SetName("inPithcBend");
		inPitchBendCfg.SetDevice("default:default");
		inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
		inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));

		mPitchBend.Configure( inPitchBendCfg );

		MIDIInConfig inBreathNoteCfg;

		inBreathNoteCfg.SetName("in2");
		inBreathNoteCfg.SetDevice("default:default");
		inBreathNoteCfg.SetChannelMask( 				
										MIDI::ChannelMask(3) |
										MIDI::ChannelMask(4)
									  );

		inBreathNoteCfg.SetMessageMask(
			MIDI::MessageMask(MIDI::eNoteOn)|
			MIDI::MessageMask(MIDI::eNoteOff)
		);

		MIDIInConfig inCtrlCfg;
		
		inCtrlCfg.SetName("inctrl");
		inCtrlCfg.SetDevice("default:default");
		inCtrlCfg.SetChannelMask(MIDI::ChannelMask(1));
		inCtrlCfg.SetMessageMask(MIDI::MessageMask(MIDI::eControlChange));
		inCtrlCfg.SetFilter(11);

		mBreathController = BreathController( inBreathNoteCfg, inCtrlCfg );

		mPitchBend.LinkOutWithInControl( 0, &mMIDIHandler, 2);

		mKeyboardNote.LinkOutWithInControl( 0, &mMIDIHandler, 1);
		mKeyboardNote.LinkOutWithInControl( 1, &mMIDIHandler, 0);
		mKeyboardNote.LinkOutWithInControl( 2, &mMIDIHandler, 1);
		mKeyboardNote.LinkOutWithInControl( 3, &mMIDIHandler, 0);

		mBreathController.mInNote.LinkOutWithInControl( 0, &mMIDIHandler, 1);
		mBreathController.mInNote.LinkOutWithInControl( 1, &mMIDIHandler, 0);
		mBreathController.mInNote.LinkOutWithInControl( 2, &mMIDIHandler, 1);
		mBreathController.mInNote.LinkOutWithInControl( 3, &mMIDIHandler, 0);

		mBreathController.mAirSpeed.LinkOutWithInControl( 0, &mMIDIHandler, 3 );
		
	}
	void Kernel::ConfigureSampleBasedIO( std::string outputFile)
	{
		AudioFileConfig outcfg;
		outcfg.SetFilename(outputFile);
		outcfg.SetFiletype(EAudioFileType::eWave);
		outcfg.SetChannels(1);

		mFileAudioOut.Configure( outcfg );

	
		mAudioManager = new AudioManager( mDSPCfg.GetSampleRate(), mDSPCfg.GetHopSize());
	
		mAudioManager->SetInternalBuffersNumber(12);

		AudioIOConfig iocfg;
		iocfg.SetName("left out");
		iocfg.SetChannelID(0);
		mAudioOut = new AudioOut(iocfg);

	}

	void Kernel::RenderSynthesis( Audio& synthbuffer )
	{
		if( mpParams->GetWriteToFile())
			mFileAudioOut.Do( synthbuffer );
		else
		{
			mAudioOut->Do( synthbuffer );
		}

	}
}
