#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "AudioFile.hxx"
#include "AudioFileHeader.hxx"
#include "AudioFileFormats.hxx"

namespace CLAMTest
{
	class AudioFileTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( AudioFileTest );

	class AudioFileTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( AudioFileTest );

		CPPUNIT_TEST( testSetLocation_FileExists_and_Is_PCM );
		CPPUNIT_TEST( testSetLocation_FileExists_and_Is_OggVorbis );
		CPPUNIT_TEST( testSetLocation_FileDoesNotExist_UnrecognizedFormat );

		CPPUNIT_TEST( testGetHeader_HeaderIsRight_PCM );
		CPPUNIT_TEST( testGetHeader_HeaderIsRight_OggVorbis );

		CPPUNIT_TEST( testGetHeader_NoHeaderWhenFileIsUnreadable );
		CPPUNIT_TEST( testGetHeader_NoHeaderWhenFileIsUnreadable_AfterOneSuccessful );


		CPPUNIT_TEST_SUITE_END();
	public:

		void setUp()
		{
			mPathToTestData = "../../../../CLAM-TestData/";
		}

		void tearDown()
		{
		}
	protected:

		std::string mPathToTestData;

	private: // test cases

		void testSetLocation_FileExists_and_Is_PCM()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );

			CPPUNIT_ASSERT_EQUAL( std::string("PCM"),
					      file.GetKind().GetString() );
		}

		void testSetLocation_FileExists_and_Is_OggVorbis()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "JannieJones-short-Stereo.ogg" ) );

			CPPUNIT_ASSERT_EQUAL( std::string("Ogg/Vorbis"),
					      file.GetKind().GetString() );			
		}

		void testSetLocation_FileDoesNotExist_UnrecognizedFormat()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string("nikora") );

			CPPUNIT_ASSERT_EQUAL( std::string( "Unknown" ),
					      file.GetKind().GetString() );
		}

		void testGetHeader_HeaderIsRight_PCM()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );

			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( int(22050),
					      int(header.GetSampleRate()) );
			CPPUNIT_ASSERT_EQUAL( 1,
					      header.GetChannels() );
			CPPUNIT_ASSERT_EQUAL( int(6072.02),
					      int(header.GetLength()) );
			CPPUNIT_ASSERT_EQUAL( std::string("WAV"),
					      header.GetFormat().GetString() );
			CPPUNIT_ASSERT_EQUAL( std::string("signed 16-bit"),
					      header.GetEncoding().GetString() );
			
			CPPUNIT_ASSERT_EQUAL( std::string("Format Default"),
					      header.GetEndianess().GetString() );
					      
					      
		}

		void testGetHeader_HeaderIsRight_OggVorbis()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "JannieJones-short-Stereo.ogg" ) );

			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( int(44100.),
					      int(header.GetSampleRate()) );
			CPPUNIT_ASSERT_EQUAL( 2,
					      header.GetChannels() );
			CPPUNIT_ASSERT_EQUAL( int(14818.7),
					      int(header.GetLength()) );
			CPPUNIT_ASSERT_EQUAL( std::string( "VorbisMk1" ),
					      header.GetFormat().GetString() );
			CPPUNIT_ASSERT_EQUAL( std::string( "Format Default" ),
					      header.GetEncoding().GetString() );
			CPPUNIT_ASSERT_EQUAL( std::string( "Format Default" ),
					      header.GetEndianess().GetString() );

		}

		void testGetHeader_NoHeaderWhenFileIsUnreadable()
		{
			CLAM::AudioFile file;
			file.SetLocation( "momonga" );
			
			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( false,
					      header.HasSampleRate() );
		}

		void testGetHeader_NoHeaderWhenFileIsUnreadable_AfterOneSuccessful()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );
			
			file.SetLocation( "Momonga" );
			
			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( false,
					      header.HasSampleRate() );
		}

	};
	
}
