
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

class SMSAnalysisTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSAnalysisTest );

class SMSAnalysisTest : public CppUnit::TestFixture
{
	
public:
	
	CPPUNIT_TEST_SUITE( SMSAnalysisTest );

	CPPUNIT_TEST( testSine_innerData );
	CPPUNIT_TEST( testSweep_innerData );
	CPPUNIT_TEST( testTrumpet_innerData );
	
	CPPUNIT_TEST_SUITE_END();

protected: // Attributes
	CLAM::SMSAnalysisCore mAnalysis;

	std::string mPathToTestData;
	CLAM::MonoAudioFileReader mReader;

	// XR: TODO change this
	DummyProcessing <CLAM::SpectralPeakArray> dummySpectralPeakArray;
	DummyProcessing <CLAM::Fundamental> dummyFundamental;
	DummyProcessing <CLAM::Spectrum > dummySpectrum;
	DummyProcessing <CLAM::Spectrum > dummyResSpectrum;
	DummyProcessing <CLAM::Spectrum > dummySinSpectrum;

	void LoadAudioFile( const std::string & audioFileName )
	{
		CLAM::AudioFile file;
		file.SetLocation( audioFileName );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		mReader.Configure( cfg );
		mReader.Start();

		CLAM::SMSAnalysisConfig cfgAnalysis;
		cfgAnalysis.SetSamplingRate( file.GetHeader().GetSampleRate() );
		mAnalysis.Stop();
		mAnalysis.Configure( cfgAnalysis );
		mAnalysis.Start();

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
		mAnalysis.Do();
	
		dummySpectralPeakArray.Do();
		dummyFundamental.Do();
		dummySpectrum.Do();
		dummyResSpectrum.Do();
		dummySinSpectrum.Do();
	}


public: // TestFixture interface

	SMSAnalysisTest()
	{	
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		if ( !pathToTestData )
			mPathToTestData ="../../../../../CLAM-TestData/"; 
		else
			mPathToTestData = pathToTestData;
		
		CLAM::TSize frameSize = 256;		

		mReader.GetOutPort("Samples read").SetSize( frameSize );
		mReader.GetOutPort("Samples read").SetHop( frameSize );

		mReader.GetOutPort("Samples read").ConnectToIn( mAnalysis.GetInPort("Input Audio"));
		mAnalysis.GetOutPort("Sinusoidal Spectral Analysis").ConnectToIn( dummySinSpectrum.GetInPort("input"));
		mAnalysis.GetOutPort("Residual Spectral Analysis").ConnectToIn( dummySpectrum.GetInPort("input"));
		mAnalysis.GetOutPort( "Fundamental").ConnectToIn( dummyFundamental.GetInPort("input"));
		mAnalysis.GetOutPort( "Sinusoidal Tracking").ConnectToIn( dummySpectralPeakArray.GetInPort( "input" ));
		mAnalysis.GetOutPort("Substracted Spectral Analysis").ConnectToIn( dummyResSpectrum.GetInPort("input"));

	}
	
	void setUp()
	{
		mReader.GetOutPort("Samples read").CenterEvenRegions();
		mAnalysis.Start();

		dummySpectralPeakArray.Start();
		dummyFundamental.Start();
		dummySpectrum.Start();
		dummyResSpectrum.Start();
		dummySinSpectrum.Start();

		
	}
	void tearDown()
	{
		mReader.Stop();	
		mAnalysis.Stop();
	
		dummySpectralPeakArray.Stop();
		dummyFundamental.Stop();
		dummySpectrum.Stop();
		dummyResSpectrum.Stop();
		dummySinSpectrum.Stop();

	}
	
	void testSine_innerData()
	{
				
		LoadAudioFile( mPathToTestData + "sine.wav" );
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
		LoadAudioFile( mPathToTestData + "sweep.wav" );
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
	
		LoadAudioFile( mPathToTestData + "trumpet.wav" );
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
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" ), 
 	 			      StringFromData( dummyResSpectrum.GetData() ) );

//??		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_trumpet.xml" ), 
  //	 			      StringFromData( dummyFundamental.GetData() ) );
	
	}


};

} // namespace CLAMTest

