
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

	}
	
	void setUp()
	{
		mReader.GetOutPort("Samples read").CenterEvenRegions();
		mAnalysis.Start();
	}
	void tearDown()
	{
		mReader.Stop();	
		mAnalysis.Stop();
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


		CLAM::OutPortBase & outSpectrum  = mAnalysis.GetOutPort("Residual Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sine.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSpectrum)) );
					      
		CLAM::OutPortBase & outSinSpectrum  = mAnalysis.GetOutPort("Sinusoidal Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sine.xml" ), 
  	 			      StringFromData(  CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSinSpectrum)) );

		CLAM::OutPortBase & outSinTracking  = mAnalysis.GetOutPort("Sinusoidal Tracking");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_sine.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::SpectralPeakArray>::GetLastWrittenData( outSinTracking)) );
	
		CLAM::OutPortBase & outFundamental  = mAnalysis.GetOutPort("Fundamental");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_sine.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Fundamental>::GetLastWrittenData( outFundamental)) );

		CLAM::OutPortBase & outSubstracted  = mAnalysis.GetOutPort("Substracted Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_sine.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSubstracted)) );

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
		CLAM::OutPortBase & outSpectrum  = mAnalysis.GetOutPort("Residual Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sweep.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSpectrum)) );
					      
		CLAM::OutPortBase & outSinSpectrum  = mAnalysis.GetOutPort("Sinusoidal Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sweep.xml" ), 
  	 			      StringFromData(  CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSinSpectrum)) );

		CLAM::OutPortBase & outSinTracking  = mAnalysis.GetOutPort("Sinusoidal Tracking");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_sweep.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::SpectralPeakArray>::GetLastWrittenData( outSinTracking)) );
	
		CLAM::OutPortBase & outFundamental  = mAnalysis.GetOutPort("Fundamental");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_sweep.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Fundamental>::GetLastWrittenData( outFundamental)) );

		CLAM::OutPortBase & outSubstracted  = mAnalysis.GetOutPort("Substracted Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_sweep.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSubstracted)) );
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
		CLAM::OutPortBase & outSpectrum  = mAnalysis.GetOutPort("Residual Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_trumpet.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSpectrum)) );
					      
		CLAM::OutPortBase & outSinSpectrum  = mAnalysis.GetOutPort("Sinusoidal Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_trumpet.xml" ), 
  	 			      StringFromData(  CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSinSpectrum)) );

		CLAM::OutPortBase & outSinTracking  = mAnalysis.GetOutPort("Sinusoidal Tracking");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_trumpet.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::SpectralPeakArray>::GetLastWrittenData( outSinTracking)) );
	
//		CLAM::OutPortBase & outFundamental  = mAnalysis.GetOutPort("Fundamental");
//		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_trumpet.xml" ), 
//	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Fundamental>::GetLastWrittenData( outFundamental)) );

		CLAM::OutPortBase & outSubstracted  = mAnalysis.GetOutPort("Substracted Spectral Analysis");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSubstracted)) );
	
	}


};

} // namespace CLAMTest

