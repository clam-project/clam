
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	

#include "AudioFile.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "Audio.hxx"
#include "Segment.hxx"
#include "SMSAnalysis.hxx"
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

	CPPUNIT_TEST( testSine );
	CPPUNIT_TEST( testTrumpet );
	CPPUNIT_TEST( testSweep );
	
	CPPUNIT_TEST_SUITE_END();

protected: // Attributes
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

public: // TestFixture interface
	void setUp()
	{
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		if ( !pathToTestData )
			mPathToTestData ="../../../../../CLAM-TestData/"; 
		else
			mPathToTestData = pathToTestData;
	}
	void tearDown()
	{
	}
	
	
	void testSine()
	{
		CLAM::Audio in;
		LoadAudioFile( in, mPathToTestData + std::string( "sine.wav" ) );
		CLAM::Segment segment;
		segment.SetAudio( in );
		CLAM::SMSAnalysis analysis;
		analysis.Start();
		analysis.Do(segment);
		analysis.Stop();

		CPPUNIT_ASSERT_EQUAL( true, CheckResultWithStoredSegment( segment, mPathToTestData + "/SMSAnalysisTests/sine.xml" ) );
	}

	void testSweep()
	{
		CLAM::Audio in;
		LoadAudioFile( in, mPathToTestData + std::string( "sweep.wav" ) );
		CLAM::Segment segment;
		segment.SetAudio( in );
		CLAM::SMSAnalysis analysis;
		analysis.Start();
		analysis.Do(segment);
		analysis.Stop();

		CPPUNIT_ASSERT_EQUAL( true, CheckResultWithStoredSegment( segment, mPathToTestData + "/SMSAnalysisTests/sweep.xml" ) );
	}
	
	void testTrumpet()
	{
		CLAM::Audio in;
		LoadAudioFile( in, mPathToTestData + std::string( "trumpet.wav" ) );
		CLAM::Segment segment;
		segment.SetAudio( in );
		CLAM::SMSAnalysis analysis;
		analysis.Start();
		analysis.Do(segment);
		analysis.Stop();

		CPPUNIT_ASSERT_EQUAL( true, CheckResultWithStoredSegment( segment, mPathToTestData + "/SMSAnalysisTests/trumpet.xml" ) );
	}

};

} // namespace CLAMTest

