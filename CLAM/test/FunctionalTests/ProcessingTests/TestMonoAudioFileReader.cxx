#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "Audio.hxx"
#include "CLAM_Math.hxx"
#include "AudioFileOpsTestsHelper.hxx"
#include "similarityHelper.hxx"
#include <algorithm>

namespace CLAMTest
{
	class MonoAudioFileReaderFunctionalTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( MonoAudioFileReaderFunctionalTest );

	class MonoAudioFileReaderFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MonoAudioFileReaderFunctionalTest );

		// Configuration values checking tests
		CPPUNIT_TEST( testConfigure_ReturnsTrueWithJustFilename );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWithoutAudioFileInConfig );
		CPPUNIT_TEST( testConfigure_ReturnsTrueWhenFileExists );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWhenFileDoesNotExist );
		CPPUNIT_TEST( testConfigure_ReturnsTrueWhenSelectedChannelExists );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWhenSelectedChannelIsOutOfRange );

		// File reading checking
		CPPUNIT_TEST( testDo_JustOneFrameFromMonoFile );
		CPPUNIT_TEST( testDo_JustTwoFramesFromMonoFile );
		CPPUNIT_TEST( testDo_JustOneFrameFromStereoFile );
		CPPUNIT_TEST( testDo_JustTwoFramesFromStereoFile );
		CPPUNIT_TEST( testDo_JustTwoFramesBeginTimesAreRight );
		CPPUNIT_TEST( testDo_JustOneFrame_SampleRateIsOK );

		CPPUNIT_TEST_SUITE_END();

	protected: // Attributes

		std::string mPathToTestData;

	protected: // Auxiliary methods

	public: // TestFixture interface

		void setUp()
		{
			mPathToTestData = "../../../../../CLAM-TestData/";
		}

		void tearDown()
		{
		}

	private: // tests cases

		void testConfigure_ReturnsTrueWithJustFilename()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "Elvis.wav" ) );
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWithoutAudioFileInConfig()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.RemoveSourceFile();
			cfg.UpdateData();
			
			CLAM::MonoAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}
		
		void testConfigure_ReturnsTrueWhenFileExists()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "Elvis.wav" ) );
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MonoAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenFileDoesNotExist()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "QWERTY.wav" ) );
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}
		
		void testConfigure_ReturnsTrueWhenSelectedChannelExists()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "Elvis.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenSelectedChannelIsOutOfRange()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "Elvis.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 32768 );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
			
		}

		void testDo_JustOneFrameFromMonoFile()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "Elvis.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamples;
			readSamples.SetSampleRate( file.GetSampleRate() );
			readSamples.SetSize( 256 );

			(*(proc.FirstOutput()))->Attach( readSamples );

			proc.Start();
			proc.Do();
			proc.Stop();

			double similarity = fileSimilarity( file, 0, readSamples );
						
			CPPUNIT_ASSERT(  similarity >= 0.9999 );
			
		}

		void testDo_JustTwoFramesFromMonoFile()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "Elvis.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( file.GetSampleRate() );
			readSamples_1.SetSize( 256 );

			CLAM::DataArray previous;
			previous.Resize( readSamples_1.GetSize() );
			previous.SetSize( readSamples_1.GetSize() );


			proc.Start();

			(*(proc.FirstOutput()))->Attach( readSamples_1 );

			proc.Do();
			
			std::copy( readSamples_1.GetBuffer().GetPtr(),
				   readSamples_1.GetBuffer().GetPtr()+readSamples_1.GetSize(),
				   previous.GetPtr() );

			proc.Do();

			proc.Stop();

			CPPUNIT_ASSERT(  fabs( -0.803968 // MRJ: Value established by inspection
					       - evaluateSimilarity( previous,
								     readSamples_1.GetBuffer()) 
						 ) < 1e-4 );

		}

		void testDo_JustOneFrameFromStereoFile()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamples;
			readSamples.SetSampleRate( file.GetSampleRate() );
			readSamples.SetSize( 256 );

			(*(proc.FirstOutput()))->Attach( readSamples );

			proc.Start();
			proc.Do();
			proc.Stop();

			double similarity = fileSimilarity( file, 1, readSamples );
						
			CPPUNIT_ASSERT(  similarity >= 0.9999 );
			
		}

		void testDo_JustTwoFramesFromStereoFile()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( file.GetSampleRate() );
			readSamples_1.SetSize( 256 );

			CLAM::DataArray previous;
			previous.Resize( readSamples_1.GetSize() );
			previous.SetSize( readSamples_1.GetSize() );


			proc.Start();

			(*(proc.FirstOutput()))->Attach( readSamples_1 );

			proc.Do();

			std::copy( readSamples_1.GetBuffer().GetPtr(),
				   readSamples_1.GetBuffer().GetPtr()+readSamples_1.GetSize(),
				   previous.GetPtr() );

			proc.Do();

			proc.Stop();

			CPPUNIT_ASSERT( evaluateSimilarity( readSamples_1.GetBuffer(),
							    previous )
					< 1.0 );
			
		}

		void testDo_JustTwoFramesBeginTimesAreRight()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( file.GetSampleRate() );
			readSamples_1.SetSize( 256 );

			CLAM::TTime previousBeginTime = 0.0;


			proc.Start();

			(*(proc.FirstOutput()))->Attach( readSamples_1 );

			proc.Do();

			previousBeginTime = readSamples_1.GetBeginTime();

			proc.Do();

			proc.Stop();

			CPPUNIT_ASSERT_EQUAL( CLAM::TTime( readSamples_1.GetSize() ) / file.GetSampleRate(),
					      readSamples_1.GetBeginTime() - previousBeginTime);


		}

		void testDo_JustOneFrame_SampleRateIsOK()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamples;
			readSamples.SetSize( 256 );

			(*(proc.FirstOutput()))->Attach( readSamples );

			proc.Start();
			proc.Do();
			proc.Stop();

			CPPUNIT_ASSERT_EQUAL( file.GetSampleRate(),
					      readSamples.GetSampleRate() );

		}

	};

}
