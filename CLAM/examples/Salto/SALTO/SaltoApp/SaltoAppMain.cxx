//#include "CSaltoParameter.hxx"
#include "Parameters.hxx"
#include "SaltoSynth.hxx"
#include "CSaltoEditor.hxx"
//#include "CSaltoMIDIInOut.hxx"
#include "Err.hxx"
#include "GUIAudioApplication.hxx"
#include "WidgetTKWrapper.hxx"
#include "AudioManager.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "AudioFileOut.hxx"
#include "MIDIHandler.hxx"
#include "MIDIBreathController.hxx"
#include "CSaltoDataManagment.hxx"
#include <iostream>

using CLAMGUI::WidgetTKWrapper;
using namespace CLAM;

class SaltoApp:public GUIAudioApplication
{
public:
	SaltoApp()
	{
		pParams=NULL;
		pDSP = NULL;
		pMelody = NULL;
		pGUI=NULL;
	}

	virtual ~SaltoApp()
	{
		if (pParams) delete pParams;
		if (pDSP) delete pDSP;
		if (pMelody) delete pMelody;
		if (pGUI) delete pGUI;
	}

	Fl_Window* CreateWindow(int argc,char** argv)
	{
		pParams = Parameters::GetInstance();
		if(pParams==NULL)
			throw Err("OOM in MAIN cant construct params");

		CSaltoDataManagment::InitSaltoDB( pParams );

		pMelody = new MelodyTranslator;
		if( pMelody == NULL )
			throw Err("OOM in MAIN cant construct melody");


		//pDSP = new CSaltoDSP(pParams);//,pMIDI);
		DSPCfg.SetParams( *pParams );
		pDSP = new SaltoSynth(DSPCfg);
		if(pDSP==NULL)
			throw Err("OOM in MAIN cant construct CSaltoDSP");

		// gui , user interface interacts also with parameter class
		pGUI = new CSaltoEditor(pParams, pDSP, pMelody, this);//,pMIDI);
		if(pGUI==NULL)
			throw Err("OOM in MAIN cant construct editor");

		MIDIHandlerConfig MIDIHandlerCfg;
		MIDIHandlerCfg.SetTranspose( pParams->GetTranspose() );
		MIDIHandlerCfg.SetPitchModRange( DSPCfg.GetPitchModRange() );
		mMIDIHandler.Configure( MIDIHandlerCfg );

		mMIDIHandler.LinkOutWithInControl( 0, pDSP, 0 );

		pMelody->LinkOutWithInControl( 0, &mMIDIHandler, 0);
		pMelody->LinkOutWithInControl( 1, &mMIDIHandler, 1);

		pGUI->mpEditorWindow->set_non_modal();
		pGUI->mpEditorWindow->show(argc,argv);


		return pGUI->mpEditorWindow;
	}

protected:

	Parameters*       pParams;
	SaltoSynth*       pDSP;
	MelodyTranslator* pMelody;
	CSaltoEditor*     pGUI;
	SaltoSynthConfig  DSPCfg;

	void UserMain(void)
	{
		WidgetTKWrapper* tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		Start();

		tk->Run();
	}


	void AudioMain(void)
	{

		try
		{
			bool not_finished = true;
			Audio* synthbuffer = NULL;

			ConfigureSampleBasedIO();

			pDSP->BindWithGUI( pGUI );
			pDSP->Start();

			MIDIInConfig inNoteCfg;

			inNoteCfg.SetName("in");
			inNoteCfg.SetDevice("default:default");
			MIDIInControl keyboardNote( inNoteCfg );

			MIDIInConfig inPitchBendCfg;
			
			inPitchBendCfg.SetName("inPithcBend");
			inPitchBendCfg.SetDevice("default:default");
			inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
			inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));

			MIDIInControl pitchBend( inPitchBendCfg );

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

			SALTO::BreathController breathController( inBreathNoteCfg, inCtrlCfg );

			pitchBend.LinkOutWithInControl( 0, &mMIDIHandler, 2);

			keyboardNote.LinkOutWithInControl( 0, &mMIDIHandler, 1);
			keyboardNote.LinkOutWithInControl( 1, &mMIDIHandler, 0);
			keyboardNote.LinkOutWithInControl( 2, &mMIDIHandler, 1);
			keyboardNote.LinkOutWithInControl( 3, &mMIDIHandler, 0);

			breathController.mInNote.LinkOutWithInControl( 0, &mMIDIHandler, 1 );
			breathController.mInNote.LinkOutWithInControl( 1, &mMIDIHandler, 0);
			breathController.mInNote.LinkOutWithInControl( 2, &mMIDIHandler, 1);
			breathController.mInNote.LinkOutWithInControl( 3, &mMIDIHandler, 0);

			breathController.mAirSpeed.LinkOutWithInControl( 0, &mMIDIHandler, 3 );

			mFileAudioOut.Start();
			mMIDIManager.Start();
			mAudioManager->Start();
			mMIDIHandler.Start();

			while ( not_finished )
			{

				pthread_testcancel();
				
				ProcessMIDIMessages();
				
				not_finished = pDSP->Do( synthbuffer );

				CLAM_DEBUG_ASSERT( synthbuffer != NULL, "Whooops! Synthesis buffer was void!" );

			
				RenderSynthesis( *synthbuffer );

			}

			mMIDIHandler.Stop();
			pDSP->Stop();

			if ( pParams->GetWriteToFile())
				{
					std::cout << "Closing audio File ..... " << std::endl;
					pParams->SetExit(true); // MRJ: Ya lo es no?
					mFileAudioOut.Stop();
				}


			std::cout << "exiting..." << std::endl;
		
			pthread_exit( NULL );

		}
		catch (Err err)
		{
			err.Print();
			exit(-1);
		}
	}

	void ConfigureSampleBasedIO()
	{
		AudioFileConfig outcfg;
		outcfg.SetFilename("salto.wav");
		outcfg.SetFiletype(EAudioFileType::eWave);
		outcfg.SetChannels(1);

		mFileAudioOut.Configure( outcfg );

	
		mAudioManager = new AudioManager( DSPCfg.GetSampleRate(), DSPCfg.GetHopSize());
	
		mAudioManager->SetInternalBuffersNumber(12);

		AudioIOConfig iocfgL;
		iocfgL.SetName("left out");
		iocfgL.SetChannelID(0);
		AudioIOConfig iocfgR;
		iocfgR.SetName("right out");
		iocfgR.SetChannelID(1);
		mAudioOutL = new AudioOut(iocfgL);
		mAudioOutR = new AudioOut(iocfgR);
		//iocfg.SetName("left in");
		//mAudioIn = new AudioIn(iocfg);


//		mDummyIn.SetSize( DSPCfg.GetHopSize() );
	}

	void RenderSynthesis(Audio& synthbuffer)
	{
		if( pParams->GetWriteToFile())
			mFileAudioOut.Do( synthbuffer );
		else
		{
			
			mAudioOutL->Do( synthbuffer );
			mAudioOutR->Do( synthbuffer );
		}
	}

	void SetupMIDI()
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

		MIDIInControl keyboardNote( inNoteCfg );

		MIDIInConfig inPitchBendCfg;
		
		inPitchBendCfg.SetName("inPithcBend");
		inPitchBendCfg.SetDevice("default:default");
		inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
		inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));

		MIDIInControl pitchBend( inPitchBendCfg );

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
		inCtrlCfg.SetChannelMask(MIDI::ChannelMask(2));
		inCtrlCfg.SetMessageMask(MIDI::MessageMask(MIDI::eControlChange));
		inCtrlCfg.SetFilter(0x02);

		SALTO::BreathController breathController( inBreathNoteCfg, inCtrlCfg );

		pitchBend.LinkOutWithInControl( 0, &mMIDIHandler, 2);

		keyboardNote.LinkOutWithInControl( 0, &mMIDIHandler, 1);
		keyboardNote.LinkOutWithInControl( 1, &mMIDIHandler, 0);
		keyboardNote.LinkOutWithInControl( 2, &mMIDIHandler, 1);
		keyboardNote.LinkOutWithInControl( 3, &mMIDIHandler, 0);

	}

	void ProcessMIDIMessages(void)
	{
		if (pParams->GetUseMelody())
			{
				TTime currentTime = pDSP->GetEventSample()/DSPCfg.GetSampleRate();
				if (!pMelody->Do( pParams->GetPlay(), currentTime ))
					{
						mMIDIHandler.Do( *pParams );
						pParams->SetUseMelody( false );
						pDSP->ResetEventSample();
					}
				else
					mMIDIHandler.Do( *pParams );
			}
			
		if ( pParams->GetUseMidiKeyboard() || pParams->GetUseBreathController() )
			{
				mMIDIManager.Check();
				mMIDIHandler.Do( *pParams );
			}		

		// End of SALTO MIDI Messages Processing
	}

private:

	// Sample Based
	AudioManager*    mAudioManager;
	AudioOut*        mAudioOutL;
	AudioOut*        mAudioOutR;
	AudioFileOut     mFileAudioOut;
	//MIDI based
	MIDIManager      mMIDIManager;
	MIDIHandler      mMIDIHandler;
	// Audio for the AudioIn
	//Audio            mDummyIn;


};

int main (int argc,char** argv)
{
	SaltoApp mySalto;

	try
	{

		mySalto.Run(argc,argv);
	}
	catch ( Err& e )
	{
		e.Print();
		return -1;
	}
	return 0;
}
