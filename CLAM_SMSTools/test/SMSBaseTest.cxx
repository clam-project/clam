#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "AudioCollator.hxx"  // includes audio matching algorithms and audio file helpers


#include "SMSBase.hxx"
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"

#include "Audio.hxx"
#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"


#include <fstream> // used for open(..) : we want to check if some file exists

namespace CLAMTest
{


class SMSExampleTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSExampleTest );

class SMSExampleTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SMSExampleTest );
	CPPUNIT_TEST( testhelperAudiosAreEqual_WhenDifferentSizes );
	CPPUNIT_TEST( testhelperAudiosAreEqual_WhenDiffersInASampler );
	CPPUNIT_TEST( testhelperAudiosAreEqual_AfterDefaultConstructor );
	CPPUNIT_TEST( testInitConfigs_GenerateCompatibleConfig );
	CPPUNIT_TEST( testhelperFileExist );
	CPPUNIT_TEST( testTestDataPath_TestsFilesAreAccessible );
	CPPUNIT_TEST( testLoadInputSound_WithABadFileName );
	CPPUNIT_TEST( testLoadInputSound_WithAnExistingSoundFile );
	CPPUNIT_TEST( testLoadInputSound_CalledMultipleTimes );
	CPPUNIT_TEST( testhelperLoadAudioFromFile );

	CPPUNIT_TEST( testAnalysisSynthesis_WithDefaultConfig_UsingSine_Wav );
	CPPUNIT_TEST( testAnalysisSynthesis_WithLoadedConfig_UsingSweep_Wav );
	CPPUNIT_TEST( testAnalysisSynthesis_WithLoadedConfig_UsingElvis_Wav );
	CPPUNIT_TEST( testTwoSimpleTransformations_withLoadedScore );
//	CPPUNIT_TEST( testTransformations_withLoadedScore_HarmonizerTimestreach );
	CPPUNIT_TEST( testTransformations_withLoadedScore_TimestreachMorph );
	CPPUNIT_TEST_SUITE_END();



//  TestFixture atributes:
	
	CLAM::Audio mLoadedTestAudio;

	//! relative path to the CLAM-TestData dir. Defined below the class declaration.
	std::string mPath;
	CLAM::SMSBase *mSms;

public:
	//! Common initialization, executed before each test method
	void setUp() 
	{ 
		mPath = "../../../../CLAM-TestData/";
		mSms = new CLAM::SMSBase();
	}

	//! Common clean up, executed after each test method
	void tearDown() 
	{ 
		delete mSms;
	}


private:
	
	void testhelperAudiosAreEqual_WhenDifferentSizes()
	{
		// Setup
		CLAM::Audio a, b;
		a.SetSize(2);
		b.SetSize(0);

		std::string diagnostic;
		// Verification
		bool resultEquals = helperAudiosAreEqual(a, b, diagnostic);
		CPPUNIT_ASSERT_EQUAL( std::string("different audio sizes: first 2 second 0\n"), diagnostic );
		CPPUNIT_ASSERT_EQUAL( false, resultEquals );
	}

	void testhelperAudiosAreEqual_WhenDiffersInASampler()
	{
		CLAM::Audio a, b;
		const int size = 10;
		a.SetSize(size);
		b.SetSize(size);
		CLAM::DataArray& aBuff= a.GetBuffer();
		CLAM::DataArray& bBuff= b.GetBuffer();
		const CLAM::TData _1 = (CLAM::TData) 1.0;
		const CLAM::TData _0_9 = (CLAM::TData) 0.9;
		for (int i=0; i<size; i++)
		{
			aBuff[i]=CLAM::TData(1.0);
			bBuff[i]=_1;
		}
		bBuff[size-1]=_0_9;

		std::string diagnostic;
		const bool resultEquals = helperAudiosAreEqual(a, b, diagnostic);
		CPPUNIT_ASSERT_EQUAL( std::string(
			"found a different sampler in position 9 first value is 1 second value is 0.9 with delta = 0.001"),
			diagnostic );

		CPPUNIT_ASSERT_EQUAL( false, resultEquals );
	}

	void testhelperAudiosAreEqual_AfterDefaultConstructor()
	{
		CLAM::Audio a,b;
		std::string diagnostic;
		CPPUNIT_ASSERT_EQUAL( true, helperAudiosAreEqual(a, b, diagnostic) );
		CPPUNIT_ASSERT_EQUAL( std::string(""), diagnostic );
	}
	
// ---------------------------------------------------------------------------------------------------

	// implementation of CLAM::SMSBase virtual methods 
	CLAMGUI::Progress* CreateProgress(const char* title, float from, float to)
	{
		return new CLAMGUI::StdOutProgress(title, from, to);
	}
	CLAMGUI::WaitMessage* CreateWaitMessage(const char * title)
	{
		return new CLAMGUI::StdOutWaitMessage(title);
	}

	void Run(){}
// ---------------------------------------------------------------------------------------------------

	// SMSBase tests :

	void testInitConfigs_GenerateCompatibleConfig()
	{
		mSms->InitConfigs();
		CPPUNIT_ASSERT_MESSAGE("after InitConfigs, HaveCompatibleConfig() should be true", 
				mSms->HaveCompatibleConfig() );
	}


	void testhelperFileExist()
	{

		CPPUNIT_ASSERT_MESSAGE("current dir should contain file: settings.cfg",
				helperFileExist("settings.cfg") );
		CPPUNIT_ASSERT_EQUAL( false, helperFileExist("thisFileShouldNot.Exist") );
	}

	void testTestDataPath_TestsFilesAreAccessible()
	{
		
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening sweep.wav."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"sweep.wav" ) );

		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig.wav."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"/SMSTests/out_sweep_defConfig.wav") );
		
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig_res.wav."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"/SMSTests/out_sweep_defConfig_res.wav") );
		
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig_sin.wav."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"/SMSTests/out_sweep_defConfig_sin.wav") );

		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/config.xml."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"/SMSTests/sweepConfig.xml") );
		
	}

	void testLoadInputSound_WithABadFileName()
	{
		mSms->InitConfigs();
		mSms->SetInputSoundFile("thisFileShouldAbsolutelyNotExist.wav");
		CPPUNIT_ASSERT_EQUAL( false, mSms->LoadInputSound() );
		mSms->SetInputSoundFile("");

	}

	void testLoadInputSound_WithAnExistingSoundFile()
	{
		mSms->InitConfigs();
		mSms->SetInputSoundFile( mPath+"sweep.wav");
		CPPUNIT_ASSERT_EQUAL( true, mSms->LoadInputSound() );
		mSms->SetInputSoundFile("");
	}

	void testLoadInputSound_CalledMultipleTimes()
	{
		mSms->InitConfigs();
		mSms->SetInputSoundFile( mPath+"sweep.wav");
		CPPUNIT_ASSERT_EQUAL( true, mSms->LoadInputSound() );
		CPPUNIT_ASSERT_EQUAL( true, mSms->LoadInputSound() );
		mSms->SetInputSoundFile("");
	}
	

	CLAM::Audio& accessorOriginalAudio()
	{
		return mSms->GetOriginalAudio();
	}
	CLAM::Audio& accessorSynthesizedAudio()
	{
		return mSms->GetSynthesizedSound();
	}
	CLAM::Audio& accessorSinusoidalAudio()
	{
		return mSms->GetSynthesizedSinusoidal();
	}
	CLAM::Audio& accessorResidualAudio()
	{
		return mSms->GetSynthesizedResidual();
	}



	CLAM::Audio& accessorLoadedTestAudio()
	{
		return mLoadedTestAudio;
	}

	
	void testhelperLoadAudioFromFile()
	{
		CLAM::Audio loaded;
		helperLoadAudioFromFile( mPath+"sweep.wav", loaded );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("sweep.wav size", 181588, loaded.GetSize() );
	}
	
	void Assert_SynthSinusoidalAndResidual_AreEqualToExpected( 
			const std::string & baseFileName, double delta = 0.001)
	{
		std::string diagnosticSynth, diagnosticRes, diagnosticSin;
		
		bool synthAudiosAreEqual = 	
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), 
					baseFileName +".wav", diagnosticSynth, delta);
	
		bool resAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorResidualAudio(), 
					baseFileName +"_res.wav", diagnosticRes, delta);
		
		bool sinAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorSinusoidalAudio(), 
					baseFileName +"_sin.wav", diagnosticSin, delta);
		
		CPPUNIT_ASSERT_MESSAGE( diagnosticSynth + diagnosticRes + diagnosticSin, 
				synthAudiosAreEqual && resAudiosAreEqual && sinAudiosAreEqual );
	}
	
	void Assert_Synthesized_IsEqualToExpected( 
			const std::string & baseFileName, double delta = 0.001)
	{
		std::string diagnosticSynth;		
		bool synthAudiosAreEqual = 	
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), 
					baseFileName +".wav", diagnosticSynth, delta);
	
		CPPUNIT_ASSERT_MESSAGE( diagnosticSynth, synthAudiosAreEqual );
	}


	//-------------------------------------------------------------------------

	void testAnalysisSynthesis_WithDefaultConfig_UsingSine_Wav()
	{
		// TODO: make it with default config	
		mSms->LoadConfig( mPath + "/SMSTests/sweepConfig.xml");
		mSms->SetInputSoundFile( mPath+"sine.wav");
		mSms->InitConfigs();
		mSms->LoadInputSound();
		mSms->Analyze();
		mSms->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_sine_defaultConfig";		
		double delta=0.09;
		Assert_SynthSinusoidalAndResidual_AreEqualToExpected( expectedAudioFile, delta);
	}

	
	void testAnalysisSynthesis_WithLoadedConfig_UsingSweep_Wav()
	{
		mSms->LoadConfig( mPath + "/SMSTests/sweepConfig.xml");
		mSms->InitConfigs();
		mSms->LoadInputSound();
		mSms->Analyze();
		mSms->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_sweep_loadedConfig";
		double delta=0.09;
		Assert_SynthSinusoidalAndResidual_AreEqualToExpected( expectedAudioFile, delta);
	}

	void testAnalysisSynthesis_WithLoadedConfig_UsingElvis_Wav()
	{
		mSms->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mSms->InitConfigs();
		mSms->LoadInputSound();
		mSms->Analyze();
		mSms->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_elvis_loadedConfig";
		double delta=0.09;
		Assert_SynthSinusoidalAndResidual_AreEqualToExpected( expectedAudioFile, delta);
	}

	void testTwoSimpleTransformations_withLoadedScore()
	{ 
		mSms->LoadTransformationScore( mPath + "/SMSTests/wierdfemale-transf.xml" );
		mSms->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mSms->InitConfigs();
		mSms->LoadInputSound();
		mSms->Analyze();
		mSms->Transform();
		mSms->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_wierdfemale-transf";
		double delta = 0.09;
		Assert_Synthesized_IsEqualToExpected( expectedAudioFile, delta );
	}

	void testTransformations_withLoadedScore_HarmonizerTimestreach()
	{ 
		mSms->LoadTransformationScore( mPath + "/SMSTests/harmonizer_timestreach-transf.xml" );
		mSms->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mSms->InitConfigs();
		mSms->LoadInputSound();
		mSms->Analyze();
		mSms->Transform();
		mSms->Synthesize();

		CLAM::DataArray& buf = accessorSynthesizedAudio().GetBuffer();
		std::cout << "test transform 1: synth audio\n";
		std::cout << "synth audio 10 " << buf[10] << std::endl; 
		std::cout << "synth audio 100 " << buf[100] << std::endl; 
		std::cout << "synth audio 500 " << buf[500] << std::endl; 
		std::cout << "synth audio 1000 " << buf[1000] << std::endl; 
			
		const std::string expectedAudioFile = mPath+"/SMSTests/out_harmonizer-timestreach-transf";
		double delta = 0.09;
		Assert_Synthesized_IsEqualToExpected( expectedAudioFile, delta );
	}
	
	void testTransformations_withLoadedScore_TimestreachMorph()
	{ 
		mSms->LoadTransformationScore( mPath + "/SMSTests/timestreach_morph-transf.xml" );
		mSms->LoadConfig( mPath + "/SMSTests/elvisMorphConfig.xml");
		mSms->InitConfigs();
		mSms->LoadInputSound();
		mSms->Analyze();
		mSms->Transform();
		mSms->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_timestreach_morph-transf";
		double delta = 0.09;
		Assert_Synthesized_IsEqualToExpected( expectedAudioFile, delta );
	}
};




} // namespace CLAMTest
