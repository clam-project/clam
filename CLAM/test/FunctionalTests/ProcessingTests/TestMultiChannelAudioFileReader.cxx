#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "AudioFile.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "Audio.hxx"
#include "CLAM_Math.hxx"
#include "AudioFileOpsTestsHelper.hxx"
#include "similarityHelper.hxx"
#include <algorithm>
#include <iterator>

namespace CLAMTest
{
	class MultiChannelAudioFileReaderFunctionalTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( MultiChannelAudioFileReaderFunctionalTest );

	class MultiChannelAudioFileReaderFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MultiChannelAudioFileReaderFunctionalTest );

		// Configuration values checking tests
		CPPUNIT_TEST( testConfigure_ReturnsTrueWithJustFilename );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWithoutAudioFileInConfig );
		CPPUNIT_TEST( testConfigure_ReturnsTrueWhenFileExists );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWhenFileDoesNotExist );
		CPPUNIT_TEST( testConfigure_DefaultChannelSelection_ChannelsOK );
		CPPUNIT_TEST( testConfigure_DefaultChannelSelection_PortNumberIsOK );
		CPPUNIT_TEST( testConfigure_ManualChannelSelection_ChannelsOK );
		CPPUNIT_TEST( testConfigure_ManualChannelSelection_PortNumberIsOK );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_ManualChannelSelection_TooMany );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_ManualChannelSelection_InvalidChannel );

		// Do test cases
		CPPUNIT_TEST( testDo_JustOneFrame );
		CPPUNIT_TEST( testDo_JustTwoFrames );
		CPPUNIT_TEST( testDo_JustOneFrame_SampleRateIsOK );
		CPPUNIT_TEST( testDo_JustTwoFrames_BeginTimesAreOK );

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
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWithoutAudioFileInConfig()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.RemoveSourceFile();
			cfg.UpdateData();
			
			CLAM::MultiChannelAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}

		void testConfigure_ReturnsTrueWhenFileExists()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenFileDoesNotExist()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile-false.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}

		void  testConfigure_DefaultChannelSelection_ChannelsOK()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );

			CLAM::MultiChannelAudioFileReaderConfig cfgToBeChecked =
				dynamic_cast< const CLAM::MultiChannelAudioFileReaderConfig& > ( proc.GetConfig() );
			
			CLAM::Array< CLAM::TIndex >& channels =
				cfgToBeChecked.GetSelectedChannels();

			bool allChannelsPresent = true;

			file.RetrieveHeaderInfo();

			if ( channels.Size() == file.GetChannels() )
			{
				for ( int i = 0; i < channels.Size(); i++ )
					allChannelsPresent &= (channels[i] == i);
			}
			else
				allChannelsPresent = false;

			CPPUNIT_ASSERT_EQUAL( true,
					      allChannelsPresent );
			
		}

		void testConfigure_DefaultChannelSelection_PortNumberIsOK()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( 2,
					      std::distance( proc.FirstOutput(),
							     proc.LastOutput() ) );
			
		}
		
		void testConfigure_ManualChannelSelection_ChannelsOK()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );

			CLAM::MultiChannelAudioFileReaderConfig cfgToBeChecked =
				dynamic_cast< const CLAM::MultiChannelAudioFileReaderConfig& > ( proc.GetConfig() );
			
			CLAM::Array< CLAM::TIndex >& channels =
				cfgToBeChecked.GetSelectedChannels();

			bool allChannelsPresent = true;

			file.RetrieveHeaderInfo();

			if ( channels.Size() == cfg.GetSelectedChannels().Size() )
			{
				for ( int i = 0; i < channels.Size(); i++ )
					allChannelsPresent &= (channels[i] == cfg.GetSelectedChannels()[i] );
			}
			else
				allChannelsPresent = false;

			CPPUNIT_ASSERT_EQUAL( true,
					      allChannelsPresent );
			
		}

		void testConfigure_ManualChannelSelection_PortNumberIsOK()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );

			CLAM::MultiChannelAudioFileReaderConfig cfg;

			cfg.AddSelectedChannels();
			cfg.UpdateData();

			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;

			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( 2,
					      std::distance( proc.FirstOutput(),
							     proc.LastOutput() ) );
		}

		void testConfigure_ReturnsFalse_ManualChannelSelection_TooMany()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 3 );
			cfg.GetSelectedChannels().SetSize( 3 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;
			cfg.GetSelectedChannels()[2] = 2;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool cfgResult = proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( false,
					      cfgResult );			
		}

		void testConfigure_ReturnsFalse_ManualChannelSelection_InvalidChannel()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 65;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool cfgResult = proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( false,
					      cfgResult );			
			
		}

		void testDo_JustOneFrame ()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			CLAM::Processing::OutPortIterator outIt =
				proc.FirstOutput();

			(*outIt++)->Attach( readSamplesLeft );
			(*outIt)->Attach( readSamplesRight );
			

			proc.Start();
			proc.Do();
			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			fileSimilarity( file, readSamplesLeft, readSamplesRight,
					similarityLeft, similarityRight );
						
			CPPUNIT_ASSERT(  similarityLeft  >= 0.9999 );
			CPPUNIT_ASSERT(  similarityRight >= 0.9999 );

		}
		
		void testDo_JustTwoFrames()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );

			CLAM::DataArray prevLeftSamples;
			prevLeftSamples.Resize( 256 );
			prevLeftSamples.SetSize( 256 );

			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			CLAM::DataArray prevRightSamples;
			prevRightSamples.Resize( 256 );
			prevRightSamples.SetSize( 256 );

			CLAM::Processing::OutPortIterator outIt =
				proc.FirstOutput();

			(*outIt++)->Attach( readSamplesLeft );
			(*outIt)->Attach( readSamplesRight );
			

			proc.Start();

			proc.Do();
			
			// Save this frame to arrays
			std::copy( readSamplesLeft.GetBuffer().GetPtr(),
				   readSamplesLeft.GetBuffer().GetPtr() 
				   + readSamplesLeft.GetBuffer().Size(),
				   prevLeftSamples.GetPtr() );
			
			std::copy( readSamplesRight.GetBuffer().GetPtr(),
				   readSamplesRight.GetBuffer().GetPtr()
				   + readSamplesRight.GetBuffer().Size(),				   
				   prevRightSamples.GetPtr() );

			// call the Do again
			proc.Do();

			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			similarityLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(),
							     prevLeftSamples );
			similarityRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
							      prevRightSamples );


			CPPUNIT_ASSERT( fabs( -0.21055 // MRJ: value established by inspection
					      - similarityLeft ) < 1e-4 );
			CPPUNIT_ASSERT( fabs( -0.227788 // MRJ: value established by inspection
					      - similarityRight ) < 1e-4 );
		}

		void testDo_JustOneFrame_SampleRateIsOK()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			CLAM::Processing::OutPortIterator outIt =
				proc.FirstOutput();

			(*outIt++)->Attach( readSamplesLeft );
			(*outIt)->Attach( readSamplesRight );
			

			proc.Start();
			proc.Do();
			proc.Stop();

			
			CPPUNIT_ASSERT_EQUAL( file.GetSampleRate(),
					      readSamplesLeft.GetSampleRate() );
			CPPUNIT_ASSERT_EQUAL( file.GetSampleRate(),
					      readSamplesRight.GetSampleRate() );
		}

		void testDo_JustTwoFrames_BeginTimesAreOK()
		{
			CLAM::AudioFile file;
			file.SetName( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			file.RetrieveHeaderInfo();

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );

			CLAM::TTime firstLeftBeginTime = 0.0;

			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			CLAM::TTime firstRightBeginTime = 0.0;

			CLAM::Processing::OutPortIterator outIt =
				proc.FirstOutput();

			(*outIt++)->Attach( readSamplesLeft );
			(*outIt)->Attach( readSamplesRight );
			

			proc.Start();

			proc.Do();
			
			// Save Begin Time's

			firstLeftBeginTime = readSamplesLeft.GetBeginTime();
			firstRightBeginTime = readSamplesRight.GetBeginTime();

			// call the Do again
			proc.Do();

			proc.Stop();

			CPPUNIT_ASSERT_EQUAL( CLAM::TTime( readSamplesLeft.GetSize() ) / file.GetSampleRate(),
					      readSamplesLeft.GetBeginTime() - firstLeftBeginTime );

			CPPUNIT_ASSERT_EQUAL( CLAM::TTime( readSamplesRight.GetSize() ) / file.GetSampleRate(),
					      readSamplesRight.GetBeginTime() - firstRightBeginTime );

		}
		
	};
}
