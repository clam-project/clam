
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	

#include "AudioFile.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "Audio.hxx"
#include "Segment.hxx"
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
#include "AudioCollator.hxx"  // for helperFileExist

#include "Spectrum.hxx"
#include "SpectrumConfig.hxx" 
#include "Processing.hxx"

#include "SMSAnalysisCore.hxx"
#include "SMSAnalysis.hxx"

#include "OutPort.hxx"
#include "AudioOutPort.hxx"

namespace CLAMTest
{

class SMSAnalysisTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSAnalysisTest );

class SMSAnalysisTest : public CppUnit::TestFixture
{
public:
	
	CPPUNIT_TEST_SUITE( SMSAnalysisTest );
	
	CPPUNIT_TEST( testPathToFiles );
	CPPUNIT_TEST( testSine_comparingSegment);
	CPPUNIT_TEST( testSweep_comparingSegment );
	CPPUNIT_TEST( testTrumpet_comparingSegment );
	CPPUNIT_TEST( testSine_innerData );
	CPPUNIT_TEST( testSweep_innerData );
	CPPUNIT_TEST( testTrumpet_innerData );
	
	CPPUNIT_TEST_SUITE_END();

private: // Attributes	CLAM::SMSAnalysisCore mAnalysis;
	CLAM::SMSAnalysisCore mAnalysis;
	std::string mPathToTestData;
	std::string mPathToAnalysisTestData;
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
		std::ifstream file( nameOfFile.c_str() );
		return std::string(  (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() );
	}
	
	std::string StringFromData( CLAM::ProcessingData & data )
	{
		std::stringstream streamWithResultOfTest("");
		CLAM::XMLStorage::Dump(data, "Data", streamWithResultOfTest );
		return streamWithResultOfTest.str();
	}

	void DoReaderAndAnalysis()
	{
		mReader.Do();
		mAnalysis.Do();
	}


public: // TestFixture interface

	SMSAnalysisTest()
	{	
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		if ( !pathToTestData )
			mPathToTestData ="../../../../CLAM-TestData/"; 
		else
			mPathToTestData = pathToTestData;
	
		mPathToAnalysisTestData = mPathToTestData + "SMSAnalysisTests/";
		mReader.GetOutPort("Samples read").ConnectToIn( mAnalysis.GetInPort("Input Audio"));
		CLAM::TSize frameSize = 256;		

		mReader.GetOutPort("Samples read").SetSize( frameSize );
		mReader.GetOutPort("Samples read").SetHop( frameSize );
		//mReader.GetOutPort("Samples read").CenterEvenRegions();
	}
	
	void setUp()
	{
		mAnalysis.Start();
	}
	void tearDown()
	{
		mAnalysis.Stop();
	}


	void testPathToFiles()
	{
		std::string msg = "Cound't open this file: " + mPathToTestData+"sine.wav";
		CPPUNIT_ASSERT_MESSAGE(msg, helperFileExist(mPathToTestData+"sine.wav"));
	}

	void testSine_comparingSegment()
	{
		CLAM::AudioFile file;
		file.SetLocation( mPathToTestData + "sine.wav" );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader wholeFileReader;
		wholeFileReader.GetOutPort("Samples read").SetSize( file.GetHeader().GetSamples() );
		wholeFileReader.GetOutPort("Samples read").SetHop( file.GetHeader().GetSamples() );
		
		wholeFileReader.Configure(cfg);
		wholeFileReader.Start();
		wholeFileReader.Do();		
		wholeFileReader.Stop();

		CLAM::Segment segment;
		CLAM::OutPortBase & outAudio  = wholeFileReader.GetOutPort("Samples read" );
		segment.SetAudio( CLAM::AudioOutPort::GetLastWrittenAudio( outAudio ) );

		CLAM::SMSAnalysisConfig cfgAnalysis;
		cfgAnalysis.SetSamplingRate( file.GetHeader().GetSampleRate() );
		CLAM::SMSAnalysis analysis;
		analysis.Configure( cfgAnalysis );
		analysis.Start();
		analysis.Do( segment );
		analysis.Stop();
	
//		CLAM::XMLStorage::Dump(segment, "Data", mPathToTestData + "/SMSAnalysisTests/sine_segment.xml" );
		
		CPPUNIT_ASSERT_EQUAL( 
				StringFromFile(mPathToTestData+"/SMSAnalysisTests/sine_segment.xml" ), 
				StringFromData(segment) );
	}
	
	void testSweep_comparingSegment()
	{
		CLAM::AudioFile file;
		file.SetLocation( mPathToTestData + "sweep.wav" );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader wholeFileReader;
		wholeFileReader.GetOutPort( "Samples read" ).SetSize( file.GetHeader().GetSamples() );
		wholeFileReader.GetOutPort( "Samples read" ).SetHop( file.GetHeader().GetSamples() );
		
		wholeFileReader.Configure( cfg );
		wholeFileReader.Start();
		wholeFileReader.Do();		
		wholeFileReader.Stop();

		CLAM::Segment segment;
		CLAM::OutPortBase & outAudio  = wholeFileReader.GetOutPort("Samples read" );
		segment.SetAudio( CLAM::AudioOutPort::GetLastWrittenAudio( outAudio ) );

		CLAM::SMSAnalysisConfig cfgAnalysis;
		cfgAnalysis.SetSamplingRate( file.GetHeader().GetSampleRate() );
		CLAM::SMSAnalysis analysis;
		analysis.Configure( cfgAnalysis );
		analysis.Start();
		analysis.Do( segment );
		analysis.Stop();
	
//		CLAM::XMLStorage::Dump(segment, "Data", mPathToTestData + "/SMSAnalysisTests/sweep_segment_ports.xml" );
		
		CPPUNIT_ASSERT_EQUAL( 
				StringFromFile(mPathToTestData+"/SMSAnalysisTests/sweep_segment.xml"), 
				StringFromData(segment) );
	}
	
	void testTrumpet_comparingSegment()
	{
		CLAM::AudioFile file;
		file.SetLocation( mPathToTestData + "trumpet.wav" );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader wholeFileReader;
		wholeFileReader.GetOutPort( "Samples read" ).SetSize( file.GetHeader().GetSamples() );
		wholeFileReader.GetOutPort( "Samples read" ).SetHop( file.GetHeader().GetSamples() );
		
		wholeFileReader.Configure( cfg );
		wholeFileReader.Start();
		wholeFileReader.Do();		
		wholeFileReader.Stop();

		CLAM::Segment segment;
		CLAM::OutPortBase & outAudio  = wholeFileReader.GetOutPort("Samples read" );
		segment.SetAudio( CLAM::AudioOutPort::GetLastWrittenAudio( outAudio ) );

		CLAM::SMSAnalysisConfig cfgAnalysis;
		cfgAnalysis.SetSamplingRate( file.GetHeader().GetSampleRate() );
		CLAM::SMSAnalysis analysis;
		analysis.Configure( cfgAnalysis );
		analysis.Start();
		analysis.Do( segment );
		analysis.Stop();
	
//		CLAM::XMLStorage::Dump(segment, "Data", mPathToTestData + "/SMSAnalysisTests/trumpet_segment_ports.xml" );
		
		CPPUNIT_ASSERT_EQUAL( 
				StringFromFile(mPathToTestData+"/SMSAnalysisTests/trumpet_segment.xml"), 
				StringFromData(segment) );
	}
	
	void testSine_innerData()
	{
				
		LoadAudioFile( mPathToTestData + "sine.wav" );
		DoReaderAndAnalysis();
		DoReaderAndAnalysis();

	/*	// For getting the "good" xml files	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sine.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sine.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_sine.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_sine.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_sine.xml" );
	*/

		{
			CLAM::OutPortBase & outPort  = mAnalysis.GetOutPort("Residual Branch Spectrum");
			CLAM::Spectrum & out = CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( 
					StringFromFile(mPathToAnalysisTestData+"outputSpectrum_sine.xml" ), 
					StringFromData(out ) );
		}
		{
			CLAM::OutPortBase & outPort = mAnalysis.GetOutPort("Sinusoidal Branch Spectrum");
			CLAM::Spectrum & out = CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( 
					StringFromFile( mPathToAnalysisTestData+"outputSinSpectrum_sine.xml" ), 
					StringFromData(out) );
		}	
		{
			CLAM::OutPortBase & outPort  = mAnalysis.GetOutPort("Sinusoidal Peaks");
			CLAM::SpectralPeakArray & out = 
				CLAM::OutPortPublisher<CLAM::SpectralPeakArray>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( 
					StringFromFile( mPathToAnalysisTestData+"outputSpectralPeaks_sine.xml" ), 
					StringFromData(out) );
		}
		{
			CLAM::OutPortBase & outPort  = mAnalysis.GetOutPort("Fundamental");
			CLAM::Fundamental & out = CLAM::OutPortPublisher<CLAM::Fundamental>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToAnalysisTestData+"outputFundamental_sine.xml" ), 
					      StringFromData(out) );
		}
		{
			CLAM::OutPortBase & outPort  = mAnalysis.GetOutPort("Residual Spectrum");
			CLAM::Spectrum & out = CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToAnalysisTestData+"outputResSpectrum_sine.xml" ), 
					      StringFromData(out) ); 
		}

		mReader.Stop();
	}

	void testSweep_innerData()
	{	
		LoadAudioFile( mPathToTestData + "sweep.wav" );
		DoReaderAndAnalysis();
		DoReaderAndAnalysis();

	/*	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sweep.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sweep.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_sweep.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_sweep.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_sweep.xml" );
	*/	
		{
			CLAM::OutPortBase & outPort  = mAnalysis.GetOutPort("Residual Branch Spectrum");
			CLAM::Spectrum & out = CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( 
					StringFromFile(mPathToAnalysisTestData+"outputSpectrum_sweep.xml" ), 
					StringFromData(out ) );
		}
		{
			CLAM::OutPortBase & outPort = mAnalysis.GetOutPort("Sinusoidal Branch Spectrum");
			CLAM::Spectrum & out = CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( 
					StringFromFile( mPathToAnalysisTestData+"outputSinSpectrum_sweep.xml" ), 
					StringFromData(out) );
		}	
		{
			CLAM::OutPortBase & outPort  = mAnalysis.GetOutPort("Sinusoidal Peaks");
			CLAM::SpectralPeakArray & out = 
				CLAM::OutPortPublisher<CLAM::SpectralPeakArray>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( 
					StringFromFile( mPathToAnalysisTestData+"outputSpectralPeaks_sweep.xml" ), 
					StringFromData(out) );
		}
		{
			CLAM::OutPortBase & outPort  = mAnalysis.GetOutPort("Fundamental");
			CLAM::Fundamental & out = CLAM::OutPortPublisher<CLAM::Fundamental>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToAnalysisTestData+"outputFundamental_sweep.xml" ), 
					      StringFromData(out) );
		}
		{
			CLAM::OutPortBase & outPort  = mAnalysis.GetOutPort("Residual Spectrum");
			CLAM::Spectrum & out = CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outPort);
			CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToAnalysisTestData+"outputResSpectrum_sweep.xml" ), 
					      StringFromData(out) ); 
		}
		mReader.Stop();
	}
	
	void testTrumpet_innerData()
	{
		LoadAudioFile( mPathToTestData + "trumpet.wav" );
		DoReaderAndAnalysis();
		DoReaderAndAnalysis();
		
	/*	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" );
	*/
		CLAM::OutPortBase & outSpectrumPort  = mAnalysis.GetOutPort("Residual Branch Spectrum");
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_trumpet.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSpectrumPort)) );
					      
		CLAM::OutPortBase & outSinSpectrum  = mAnalysis.GetOutPort("Sinusoidal Branch Spectrum");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_trumpet.xml" ), 
  	 			      StringFromData(  CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSinSpectrum)) );

		CLAM::OutPortBase & outSinTracking  = mAnalysis.GetOutPort("Sinusoidal Peaks");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_trumpet.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::SpectralPeakArray>::GetLastWrittenData( outSinTracking)) );
	
		CLAM::OutPortBase & outFundamental  = mAnalysis.GetOutPort("Fundamental");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_trumpet.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Fundamental>::GetLastWrittenData( outFundamental)) );

		CLAM::OutPortBase & outSubstracted  = mAnalysis.GetOutPort("Residual Spectrum");
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" ), 
	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSubstracted)) );
		mReader.Stop();
	}

	
};

} // namespace CLAMTest

