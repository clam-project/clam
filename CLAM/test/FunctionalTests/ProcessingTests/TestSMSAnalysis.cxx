
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	

#include "AudioFile.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "Audio.hxx"
#include "Segment.hxx"
//#include "SMSAnalysis.hxx"
#include "XMLStorage.hxx"
#include <string>
#include <fstream>


#include "SpectralAnalysis.hxx"
#include "SpectralAnalysisConfig.hxx"
#include "SpectralPeakDetect.hxx"
#include "FundFreqDetect.hxx"
#include "SinTracking.hxx"
#include "SynthSineSpectrum.hxx"
#include "SpectrumSubstracter2.hxx"

#include "Spectrum.hxx"
#include "SpectrumConfig.hxx" 
#include "Processing.hxx"

#include "SMSAnalysisCore.hxx"

namespace CLAMTest
{

class DummyProcessingConfig : public CLAM::ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE( DummyProcessingConfig, 0, CLAM::ProcessingConfig );
};

template< class Data>
class DummyProcessing : public CLAM::Processing
{
	DummyProcessingConfig mConfig;
	CLAM::InPort<Data> mInput;
	Data mData;	
public: // methods
	DummyProcessing(): mInput( "input", this )
	{
		Configure(DummyProcessingConfig());
	}
	
	// Supervised mode Do method
	bool Do()
	{
		mData = mInput.GetData();
		mInput.Consume();
	}

	Data & GetData()
	{
		return mData;
	}
	
	// Required accessor to current Processing object configuration
	const CLAM::ProcessingConfig& GetConfig() const{ return mConfig;}		
	
	// Required accessor to the class name for objects of the DummyProcessing type
	const char* GetClassName() const{return "Dummy";}

	bool ConcreteConfigure( const CLAM::ProcessingConfig& cfg)
	{
		CopyAsConcreteConfig( mConfig, cfg );
	}
};

class AudioDummyProcessing : public CLAM::Processing
{
	DummyProcessingConfig mConfig;
	CLAM::AudioInPort mInput;
	CLAM::Audio mData;	
public: // methods
	AudioDummyProcessing(): mInput( "input", this )
	{
		Configure(DummyProcessingConfig());
	}
	
	// Supervised mode Do method
	bool Do()
	{
		mData.SetSize( mInput.GetAudio().GetSize());
		mData = mInput.GetAudio();
		mInput.Consume();
	}

	CLAM::Audio & GetData()
	{
		return mData;
	}
	
	// Required accessor to current Processing object configuration
	const CLAM::ProcessingConfig& GetConfig() const{ return mConfig;}		
	
	// Required accessor to the class name for objects of the DummyProcessing type
	const char* GetClassName() const{return "Dummy";}

	bool ConcreteConfigure( const CLAM::ProcessingConfig& cfg)
	{
		CopyAsConcreteConfig( mConfig, cfg );
	}
};


class SMSAnalysisTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSAnalysisTest );

class SMSAnalysisTest : public CppUnit::TestFixture
{
public:
	
	CPPUNIT_TEST_SUITE( SMSAnalysisTest );

	CPPUNIT_TEST( testSine_innerData );
	CPPUNIT_TEST( testSine_innerData_usingSMSAnalysisCore );
	CPPUNIT_TEST( testSweep_innerData );
	CPPUNIT_TEST( testTrumpet_innerData );
	
	CPPUNIT_TEST_SUITE_END();

protected: // Attributes
	std::string mPathToTestData;
	CLAM::SpectralAnalysis mSinSpectralAnalysis;
	CLAM::SpectralAnalysis mResSpectralAnalysis;
	CLAM::SpectralPeakDetect mPeakDetect;
	CLAM::FundFreqDetect mFundFreqDetect;
	CLAM::SinTracking mSinTracking;
	CLAM::SynthSineSpectrum mSynthSineSpectrum;
	CLAM::SpectrumSubstracter2 mSpectrumSubstracter2;
	CLAM::MonoAudioFileReader mReader;

	// XR: TODO change this
	DummyProcessing <CLAM::SpectralPeakArray> dummySpectralPeakArray;
	DummyProcessing <CLAM::Fundamental> dummyFundamental;
	DummyProcessing <CLAM::Spectrum > dummySpectrum;
	DummyProcessing <CLAM::Spectrum > dummyResSpectrum;
	DummyProcessing <CLAM::Spectrum > dummySinSpectrum;
	AudioDummyProcessing dummyAudio;

	void LoadAudioFile( const std::string & audioFileName )
	{
		CLAM::AudioFile file;
		file.SetLocation( audioFileName );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		mReader.Configure( cfg );
		mReader.Start();
	}

	bool CheckResultWithStoredSegment( CLAM::Segment & segment, const std::string fileToLoad )
	{
		std::stringstream streamWithResultOfTest("");
		CLAM::XMLStorage::Dump(segment, "Segment", streamWithResultOfTest );

		CLAM::Segment inputSegment;
		CLAM::XMLStorage::Restore( inputSegment, fileToLoad );
		std::stringstream streamLoaded("");
		CLAM::XMLStorage::Dump( inputSegment, "Segment", streamLoaded );

		std::string result( streamWithResultOfTest.str() );
		std::string toCheck( streamLoaded.str() );

		return (result == toCheck);
	}

	std::string StringFromFile( const std::string & nameOfFile )
	{
		std::stringstream stream;
		std::ifstream file( nameOfFile.c_str() );
		return std::string(  (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() );
	}
	
	std::string StringFromData( CLAM::ProcessingData & data )
	{
		std::stringstream streamWithResultOfTest("");
		CLAM::XMLStorage::Dump(data, "Data", streamWithResultOfTest );
		return streamWithResultOfTest.str();
	}

	void Execute()
	{
		mReader.Do();
		dummyAudio.Do();
		
		mSinSpectralAnalysis.Do();
		mPeakDetect.Do();
		mFundFreqDetect.Do();
		mSinTracking.Do();
		mSynthSineSpectrum.Do();
		
		mResSpectralAnalysis.Do();
		mSpectrumSubstracter2.Do();

		dummySpectralPeakArray.Do();
		dummyFundamental.Do();
		dummySpectrum.Do();
		dummyResSpectrum.Do();
		dummySinSpectrum.Do();
	}


public: // TestFixture interface

	SMSAnalysisTest()
	{
		CLAM::SpectralAnalysisConfig cfgAnalysis;
		cfgAnalysis.SetWindowType(CLAM::EWindowType::eHamming);
		mSinSpectralAnalysis.Configure( cfgAnalysis );
		cfgAnalysis.SetWindowType(CLAM::EWindowType::eBlackmanHarris92);
		mResSpectralAnalysis.Configure( cfgAnalysis );

		CLAM::SpectralPeakDetectConfig cfgPeak;
		cfgPeak.SetMagThreshold(-60);
		mPeakDetect.Configure( cfgPeak );

		CLAM::TSize sinWindowSize = 513;
		CLAM::TSize resWindowSize = 513;
		CLAM::TSize frameSize = 256;		
		CLAM::TData samplingRate = 44100.0f;

		CLAM::SpectrumConfig scfg;
		scfg.SetSize( resWindowSize/2+1);
		scfg.SetSpectralRange(samplingRate*0.5);
		CLAM::Spectrum resSpec;
		resSpec.Configure(scfg);
		scfg.SetSize(sinWindowSize/2+1);
		CLAM::Spectrum sinSpec;
		sinSpec.Configure(scfg);
	
		CLAM::SpectrumConfig Scfg; 
		CLAM::SpecTypeFlags sflags;
   		sflags.bComplex = 1;
 		sflags.bPolar = 0;
 		sflags.bMagPhase = 0;
 		sflags.bMagPhaseBPF = 0;
		Scfg.SetType(sflags);
	 	Scfg.SetSize(resSpec.GetSize());
 		Scfg.SetSpectralRange(samplingRate*0.5);
		CLAM::Spectrum tmpSpecIn(Scfg);    


		mSpectrumSubstracter2.SetPrototypes(resSpec,tmpSpecIn,resSpec);

		mReader.GetOutPort("Samples read").SetSize( frameSize );
		mReader.GetOutPort("Samples read").SetHop( frameSize );
		dummyAudio.GetInPort( "input" ).SetSize( frameSize );
		dummyAudio.GetInPort( "input" ).SetHop( frameSize );
	
		mSinSpectralAnalysis.GetInPort("Input").SetSize( sinWindowSize - 1 );
		mSinSpectralAnalysis.GetInPort("Input").SetHop( frameSize );
		mResSpectralAnalysis.GetInPort("Input").SetSize( resWindowSize - 1 );
		mResSpectralAnalysis.GetInPort("Input").SetHop( frameSize );

		mReader.GetOutPort("Samples read").ConnectToIn( mSinSpectralAnalysis.GetInPort("Input"));
		mReader.GetOutPort("Samples read").ConnectToIn( mResSpectralAnalysis.GetInPort("Input"));
		
		mSinSpectralAnalysis.GetOutPort("Output").ConnectToIn( mPeakDetect.GetInPort("Input spectrum"));
		mPeakDetect.GetOutPort( "Output spectral peak array" ).ConnectToIn( mFundFreqDetect.GetInPort("Input"));
		mPeakDetect.GetOutPort( "Output spectral peak array" ).ConnectToIn( mSinTracking.GetInPort( "Input" ));
		mFundFreqDetect.GetOutControl( "Fund Freq Value").AddLink( &(mSinTracking.GetInControl( "Fund Freq Value" )));
		mSinTracking.GetOutPort( "Output").ConnectToIn( mSynthSineSpectrum.GetInPort( "Input" ));

		mResSpectralAnalysis.GetOutPort("Output").ConnectToIn( mSpectrumSubstracter2.GetInPort( "Input 1"));
		mSynthSineSpectrum.GetOutPort("Output").ConnectToIn( mSpectrumSubstracter2.GetInPort( "Input 2"));

		mReader.GetOutPort("Samples read").ConnectToIn( dummyAudio.GetInPort("input"));
		mSinTracking.GetOutPort( "Output").ConnectToIn( dummySpectralPeakArray.GetInPort( "input" ));
		mFundFreqDetect.GetOutPort( "Output").ConnectToIn( dummyFundamental.GetInPort("input"));
		
		mSinSpectralAnalysis.GetOutPort("Output").ConnectToIn( dummySinSpectrum.GetInPort("input"));
		mResSpectralAnalysis.GetOutPort("Output").ConnectToIn( dummySpectrum.GetInPort("input"));
		mSpectrumSubstracter2.GetOutPort("Output").ConnectToIn( dummyResSpectrum.GetInPort("input"));
		

	}
	
	void setUp()
	{
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		if ( !pathToTestData )
			mPathToTestData ="../../../../../CLAM-TestData/"; 
		else
			mPathToTestData = pathToTestData;

		
		mReader.GetOutPort("Samples read").CenterEvenRegions();
		mSinSpectralAnalysis.Start();
		mPeakDetect.Start();
		mFundFreqDetect.Start();
		mSinTracking.Start();
		mSynthSineSpectrum.Start();
		mResSpectralAnalysis.Start();
		mSpectrumSubstracter2.Start();

		dummyAudio.Start();
		dummySpectralPeakArray.Start();
		dummyFundamental.Start();
		dummySpectrum.Start();
		dummyResSpectrum.Start();
		dummySinSpectrum.Start();

		
	}
	void tearDown()
	{
		mReader.Stop();	
		mSinSpectralAnalysis.Stop();
		mPeakDetect.Stop();
		mFundFreqDetect.Stop();
		mSinTracking.Stop();
		mSynthSineSpectrum.Stop();
		mResSpectralAnalysis.Stop();
		mSpectrumSubstracter2.Stop();

		dummyAudio.Stop();
		dummySpectralPeakArray.Stop();
		dummyFundamental.Stop();
		dummySpectrum.Stop();
		dummyResSpectrum.Stop();
		dummySinSpectrum.Stop();

	}
	
	void testSine_innerData()
	{
		std::string audioFileName = mPathToTestData +  "sine.wav";
		LoadAudioFile( audioFileName );


		Execute();
		Execute();
			
	/*	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sine.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sine.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_sine.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_sine.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_sine.xml" );
	*/

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sine.xml" ), 
 	 			      StringFromData( dummySpectrum.GetData() ) );

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sine.xml" ), 
  	 			      StringFromData( dummySinSpectrum.GetData()) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_sine.xml" ), 
  	 			      StringFromData( dummySpectralPeakArray.GetData() ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_sine.xml" ), 
  	 			      StringFromData( dummyFundamental.GetData() ) );
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_sine.xml" ), 
  	 			      StringFromData( dummyResSpectrum.GetData() ) );

	}

	void testSweep_innerData()
	{
		std::string audioFileName = mPathToTestData +  "sweep.wav";
		LoadAudioFile( audioFileName );


		Execute();
		Execute();
			
	/*	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sweep.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sweep.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_sweep.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_sweep.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_sweep.xml" );
	*/

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sweep.xml" ), 
 	 			      StringFromData( dummySpectrum.GetData() ) );

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sweep.xml" ), 
  	 			      StringFromData( dummySinSpectrum.GetData()) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_sweep.xml" ), 
  	 			      StringFromData( dummySpectralPeakArray.GetData() ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_sweep.xml" ), 
  	 			      StringFromData( dummyFundamental.GetData() ) );
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_sweep.xml" ), 
  	 			      StringFromData( dummyResSpectrum.GetData() ) );

	}
	
	void testTrumpet_innerData()
	{
		std::string audioFileName = mPathToTestData +  "trumpet.wav";
		LoadAudioFile( audioFileName );


		Execute();
		Execute();
			
	/*	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" );
	*/

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_trumpet.xml" ), 
 	 			      StringFromData( dummySpectrum.GetData() ) );

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_trumpet.xml" ), 
  	 			      StringFromData( dummySinSpectrum.GetData()) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_trumpet.xml" ), 
  	 			      StringFromData( dummySpectralPeakArray.GetData() ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_trumpet.xml" ), 
  	 			      StringFromData( dummyFundamental.GetData() ) );
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" ), 
  	 			      StringFromData( dummyResSpectrum.GetData() ) );

	}


	void testSine_innerData_usingSMSAnalysisCore()
	{
		CLAM::SMSAnalysisCore analysis;
		
		DummyProcessing <CLAM::SpectralPeakArray> dummySpectralPeakArray2;
		DummyProcessing <CLAM::Fundamental> dummyFundamental2;
		DummyProcessing <CLAM::Spectrum > dummySpectrum2;
		DummyProcessing <CLAM::Spectrum > dummyResSpectrum2;
		DummyProcessing <CLAM::Spectrum > dummySinSpectrum2;
		AudioDummyProcessing dummyAudio2;

		CLAM::MonoAudioFileReader reader;
		CLAM::AudioFile file;
		file.SetLocation( mPathToTestData + "sine.wav" );
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		reader.Configure( cfg );

		CLAM::TSize sinWindowSize = 513;
		CLAM::TSize resWindowSize = 513;
		CLAM::TSize frameSize = 256;		
		CLAM::TData samplingRate = 44100.0f;

		reader.GetOutPort("Samples read").SetSize( frameSize );
		reader.GetOutPort("Samples read").SetHop( frameSize );
		dummyAudio2.GetInPort( "input" ).SetSize( frameSize );
		dummyAudio2.GetInPort( "input" ).SetHop( frameSize );

		reader.GetOutPort("Samples read").ConnectToIn( analysis.GetInPort("Input Audio"));
		reader.GetOutPort("Samples read").ConnectToIn( dummyAudio2.GetInPort("input"));
		analysis.GetOutPort("Sinusoidal Spectral Analysis").ConnectToIn( dummySinSpectrum2.GetInPort("input"));
		analysis.GetOutPort("Residual Spectral Analysis").ConnectToIn( dummySpectrum2.GetInPort("input"));
		analysis.GetOutPort( "Fundamental").ConnectToIn( dummyFundamental2.GetInPort("input"));
		analysis.GetOutPort( "Sinusoidal Tracking").ConnectToIn( dummySpectralPeakArray2.GetInPort( "input" ));
		analysis.GetOutPort("Substracted Spectral Analysis").ConnectToIn( dummyResSpectrum2.GetInPort("input"));

		reader.Start();
		analysis.Start();
		dummyAudio.Start();
		dummySpectralPeakArray.Start();
		dummyFundamental.Start();
		dummySpectrum.Start();
		dummyResSpectrum.Start();
		dummySinSpectrum.Start();

		reader.Do();
		dummyAudio.Do();
		analysis.Do();
		dummySpectralPeakArray.Do();
		dummyFundamental.Do();
		dummySpectrum.Do();
		dummyResSpectrum.Do();
		dummySinSpectrum.Do();

		reader.Do();
		dummyAudio.Do();
		analysis.Do();
		dummySpectralPeakArray.Do();
		dummyFundamental.Do();
		dummySpectrum.Do();
		dummyResSpectrum.Do();
		dummySinSpectrum.Do();


		reader.Stop();
		dummyAudio.Stop();
		analysis.Stop();
		dummySpectralPeakArray.Stop();
		dummyFundamental.Stop();
		dummySpectrum.Stop();
		dummyResSpectrum.Stop();
		dummySinSpectrum.Stop();

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_trumpet.xml" ), 
 	 			      StringFromData( dummySpectrum2.GetData() ) );

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_trumpet.xml" ), 
  	 			      StringFromData( dummySinSpectrum2.GetData()) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_trumpet.xml" ), 
  	 			      StringFromData( dummySpectralPeakArray2.GetData() ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_trumpet.xml" ), 
  	 			      StringFromData( dummyFundamental.GetData() ) );
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" ), 
  	 			      StringFromData( dummyResSpectrum.GetData() ) );



	}

};

} // namespace CLAMTest

