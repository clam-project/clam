
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	

#include "AudioFile.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "Audio.hxx"
#include "Segment.hxx"
#include "SMSAnalysis.hxx"
#include "SMSAnalysisConfig.hxx"
#include "XMLStorage.hxx"
#include <string>
#include <fstream>

namespace CLAMTest
{

class SMSAnalysisTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSAnalysisTest );

class SMSAnalysisTest : public CppUnit::TestFixture
{
public:
	
	CPPUNIT_TEST_SUITE( SMSAnalysisTest );

	CPPUNIT_TEST( testSine_comparingSegment);
	CPPUNIT_TEST( testSweep_comparingSegments );
	CPPUNIT_TEST( testTrumpet_comparingSegment );
	CPPUNIT_TEST( testSine_innerData );
	CPPUNIT_TEST( testSweep_innerData );
	CPPUNIT_TEST( testTrumpet_innerData );
	
	CPPUNIT_TEST_SUITE_END();

private: // Attributes
	CLAM::SMSAnalysisConfig mConfig;
	CLAM::SMSAnalysis mAnalysis;
	int mFrameSize;
	std::string mPathToTestData;

	void LoadAudioFile( CLAM::Audio & audio, const std::string & audioFileName )
	{
		CLAM::AudioFile file;
		file.SetLocation( audioFileName );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader reader;
		reader.Configure( cfg );
	
		CLAM::TSize fileSize = file.GetHeader().GetSamples();
		audio.SetSize(fileSize);
		audio.SetSampleRate(file.GetHeader().GetSampleRate());

		//Read Audio File
		reader.Start();		
		reader.Do(audio);
		reader.Stop();

		mConfig.SetSamplingRate( file.GetHeader().GetSampleRate() );
		mAnalysis.Stop();
		mAnalysis.Configure( mConfig );
		mAnalysis.Start();
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


public: // TestFixture interface
	void setUp()
	{
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		if ( !pathToTestData )
			mPathToTestData ="../../../../../CLAM-TestData/"; 
		else
			mPathToTestData = pathToTestData;
		
	//	CLAM::XMLStorage::Dump( mConfig, "Config", mPathToTestData + "/SMSAnalysisTests/SMSAnalisisConfigUsed.xml");
		
		CLAM::XMLStorage::Restore( mConfig, mPathToTestData + "/SMSAnalysisTests/SMSAnalisisConfigUsed.xml");
		mFrameSize = mConfig.GetHopSize();
		mAnalysis.Configure(mConfig);
		mAnalysis.Start();
	}
	void tearDown()
	{
		mAnalysis.Stop();
	}
	
	void testSine_comparingSegment()
	{
		CLAM::Audio in;
		LoadAudioFile( in, mPathToTestData + std::string( "sine.wav" ) );
		CLAM::Segment segment;
		segment.SetAudio( in );
		mAnalysis.Do(segment);
	
	//	CLAM::XMLStorage::Dump(segment, "Data", mPathToTestData + "/SMSAnalysisTests/sine_segment.xml" );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/sine_segment.xml" ), StringFromData( segment ) );
	}
	
	void testSweep_comparingSegments()
	{
		CLAM::Audio in;
		LoadAudioFile( in, mPathToTestData + std::string( "sweep.wav" ) );
		CLAM::Segment segment;
		segment.SetAudio( in );
		mAnalysis.Do(segment);
	
	//	CLAM::XMLStorage::Dump(segment, "Data", mPathToTestData + "/SMSAnalysisTests/sweep_segment.xml" );
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/sweep_segment.xml" ), StringFromData( segment ) );
	}
	
	void testTrumpet_comparingSegment()
	{
		CLAM::Audio in;
		LoadAudioFile( in, mPathToTestData + std::string( "trumpet.wav" ) );
		CLAM::Segment segment;
		segment.SetAudio( in );
		mAnalysis.Do(segment);

	//	CLAM::XMLStorage::Dump(segment, "Data", mPathToTestData + "/SMSAnalysisTests/trumpet_segment.xml" );

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/trumpet_segment.xml" ), StringFromData( segment ) );
	}

	void testSine_innerData()
	{
		CLAM::Audio inputAudio;
		CLAM::Spectrum outputSpectrum;
		CLAM::SpectralPeakArray outputSpectralPeaks;
		CLAM::Fundamental outputFundamental;
		CLAM::Spectrum outputResSpectrum;
		CLAM::Spectrum outputSinSpectrum;

		LoadAudioFile( inputAudio, mPathToTestData + std::string( "sine.wav" ) );
		CLAM::Audio chunk;
		
		inputAudio.GetAudioChunk( 0, mFrameSize, chunk );
		mAnalysis.Do( chunk, outputSpectrum, outputSinSpectrum, outputSpectralPeaks, outputFundamental, outputResSpectrum );
		inputAudio.GetAudioChunk( mFrameSize, mFrameSize*2, chunk );
		mAnalysis.Do( chunk, outputSpectrum, outputSinSpectrum, outputSpectralPeaks, outputFundamental, outputResSpectrum );
		
	
	/*	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sine.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sine.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_sine.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_sine.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_sine.xml" );
	*/	

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sine.xml" ), 
  	 			      StringFromData( outputSpectrum ) );

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sine.xml" ), 
  	 			      StringFromData( outputSinSpectrum ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_sine.xml" ), 
  	 			      StringFromData( outputSpectralPeaks ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_sine.xml" ), 
  	 			      StringFromData( outputFundamental ) );
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_sine.xml" ), 
  	 			      StringFromData( outputResSpectrum ) );

	}

	void testSweep_innerData()
	{
		CLAM::Audio inputAudio;
		CLAM::Spectrum outputSpectrum;
		CLAM::SpectralPeakArray outputSpectralPeaks;
		CLAM::Fundamental outputFundamental;
		CLAM::Spectrum outputResSpectrum;
		CLAM::Spectrum outputSinSpectrum;

		LoadAudioFile( inputAudio, mPathToTestData + std::string( "sweep.wav" ) );
		CLAM::Audio chunk;
		inputAudio.GetAudioChunk( 0, mFrameSize, chunk );

		inputAudio.GetAudioChunk( 0, mFrameSize, chunk );
		mAnalysis.Do( chunk, outputSpectrum, outputSinSpectrum, outputSpectralPeaks, outputFundamental, outputResSpectrum );
		inputAudio.GetAudioChunk( mFrameSize, mFrameSize*2, chunk );
		mAnalysis.Do( chunk, outputSpectrum, outputSinSpectrum, outputSpectralPeaks, outputFundamental, outputResSpectrum );
		
	/*	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sweep.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sweep.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_sweep.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_sweep.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_sweep.xml" );
	*/	
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sweep.xml" ), 
  	 			      StringFromData( outputSpectrum ) );

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_sweep.xml" ), 
  	 			      StringFromData( outputSinSpectrum ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_sweep.xml" ), 
  	 			      StringFromData( outputSpectralPeaks ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_sweep.xml" ), 
  	 			      StringFromData( outputFundamental ) );
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_sweep.xml" ), 
  	 			      StringFromData( outputResSpectrum ) );

	}

	void testTrumpet_innerData()
	{	
		CLAM::Audio inputAudio;
		CLAM::Spectrum outputSpectrum;
		CLAM::SpectralPeakArray outputSpectralPeaks;
		CLAM::Fundamental outputFundamental;
		CLAM::Spectrum outputResSpectrum;
		CLAM::Spectrum outputSinSpectrum;

		LoadAudioFile( inputAudio, mPathToTestData + std::string( "trumpet.wav" ) );
		CLAM::Audio chunk;
		inputAudio.GetAudioChunk( 0, mFrameSize, chunk );
		mAnalysis.Do( chunk, outputSpectrum, outputSinSpectrum, outputSpectralPeaks, outputFundamental, outputResSpectrum );
		inputAudio.GetAudioChunk( mFrameSize, mFrameSize*2, chunk );
		mAnalysis.Do( chunk, outputSpectrum, outputSinSpectrum, outputSpectralPeaks, outputFundamental, outputResSpectrum );
	
	/*	
		CLAM::XMLStorage::Dump(outputSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSpectrum_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputSinSpectrum, "Data", mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputSpectralPeaks, "Data",mPathToTestData +  "/SMSAnalysisTests/outputSpectralPeaks_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputFundamental, "Data",mPathToTestData +  "/SMSAnalysisTests/outputFundamental_trumpet.xml" );
		CLAM::XMLStorage::Dump(outputResSpectrum, "Data",mPathToTestData +  "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" );
	*/	

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_trumpet.xml" ), 
  	 			      StringFromData( outputSpectrum ) );

		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSinSpectrum_trumpet.xml" ), 
  	 			      StringFromData( outputSinSpectrum ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectralPeaks_trumpet.xml" ), 
  	 			      StringFromData( outputSpectralPeaks ) );
		
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputFundamental_trumpet.xml" ), 
  	 			      StringFromData( outputFundamental ) );
	
		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputResSpectrum_trumpet.xml" ), 
  	 			      StringFromData( outputResSpectrum ) );

	}

};

} // namespace CLAMTest

