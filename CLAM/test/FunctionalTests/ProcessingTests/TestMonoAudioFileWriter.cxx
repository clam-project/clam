#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileWriterConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "Audio.hxx"
#include "similarityHelper.hxx"

namespace CLAMTest
{
	class MonoAudioFileWriterFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( MonoAudioFileWriterFunctionalTest );
	
	class MonoAudioFileWriterFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MonoAudioFileWriterFunctionalTest );
		
		CPPUNIT_TEST( testConfigure_ReturnsFalse_WithJustFilename );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_SampleRateNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_ChannelsNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_FormatNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_EncodingNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsTrue_EndianessNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsTrue_WithReasonableConfig );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_TooManyChannels );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_TooFewChannels );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_FormatAndEncodingIncompatibility );

		CPPUNIT_TEST( testDo_WritesTheSameItWasRead );

		CPPUNIT_TEST_SUITE_END();

	protected:
		
		std::string mPathToTestData;

	public:
		void setUp()
		{
			mPathToTestData ="../../../../../CLAM-TestData/";
		}

		void tearDown()
		{
		}

	private:
		
		void testConfigure_ReturnsFalse_WithJustFilename()
		{
			CLAM::AudioFile file;
			file.SetName( std::string( "NewFile.wav" ) );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );
		}

		void testConfigure_ReturnsFalse_SampleRateNotPresent()
		{
			CLAM::AudioFile file;
			file.SetName( std::string( "NewFile.wav" ) );
			file.AddChannels();
			file.AddFormat();
			file.AddEncoding();
			file.AddEndianess();
			file.UpdateData();
			file.SetChannels( 1 );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );

		}

		void testConfigure_ReturnsFalse_ChannelsNotPresent()
		{
			CLAM::AudioFile file;
			file.SetName( std::string( "NewFile.wav" ) );
			file.AddSampleRate();
			file.AddFormat();
			file.AddEncoding();
			file.AddEndianess();
			file.UpdateData();

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );

		}

		void testConfigure_ReturnsFalse_FormatNotPresent()
		{
			CLAM::AudioFile file;
			file.SetName( std::string( "NewFile.wav" ) );
			file.AddChannels();
			file.AddSampleRate();
			file.AddEncoding();
			file.AddEndianess();
			file.UpdateData();
			file.SetChannels( 1 );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );

		}

		void testConfigure_ReturnsFalse_EncodingNotPresent()
		{
			CLAM::AudioFile file;
			file.SetName( std::string( "NewFile.wav" ) );
			file.AddSampleRate();
			file.AddChannels();
			file.AddFormat();
			file.AddEndianess();
			file.UpdateData();
			file.SetChannels( 1 );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );

		}

		void testConfigure_ReturnsTrue_EndianessNotPresent()
		{
			CLAM::AudioFile file;
			file.SetName( std::string( "NewFile.wav" ) );
			file.AddSampleRate();
			file.AddChannels();
			file.AddFormat();
			file.AddEncoding();
			file.UpdateData();
			file.SetChannels( 1 );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true,
					      configResult );			
		}

		void testConfigure_ReturnsTrue_WithReasonableConfig()
		{
			CLAM::AudioFile file;
			file.AddAll();
			file.UpdateData();
			file.SetName( std::string( "NewFile.wav" ) );
			file.SetChannels( 1 );
			file.SetSampleRate( 22050 );
			file.SetFormat( CLAM::EAudioFileFormat::eWAV );
			file.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			file.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true,
					      configResult );
		}

		void testConfigure_ReturnsFalse_TooManyChannels()
		{
			CLAM::AudioFile file;
			file.AddAll();
			file.UpdateData();
			file.SetName( std::string( "NewFile.wav" ) );
			file.SetChannels( 291 );
			file.SetSampleRate( 22050 );
			file.SetFormat( CLAM::EAudioFileFormat::eWAV );
			file.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			file.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );		
		}

		void testConfigure_ReturnsFalse_TooFewChannels()
		{
			CLAM::AudioFile file;
			file.AddAll();
			file.UpdateData();
			file.SetName( std::string( "NewFile.wav" ) );
			file.SetChannels( 0 );
			file.SetSampleRate( 22050 );
			file.SetFormat( CLAM::EAudioFileFormat::eWAV );
			file.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			file.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );		

		}

		void testConfigure_ReturnsFalse_FormatAndEncodingIncompatibility()
		{
			CLAM::AudioFile file;
			file.AddAll();
			file.UpdateData();
			file.SetName( std::string( "NewFile.wav" ) );
			file.SetChannels( 1 );
			file.SetSampleRate( 22050 );
			file.SetFormat( CLAM::EAudioFileFormat::eWAV );
			file.SetEncoding( CLAM::EAudioFileEncoding::eDWVW_24 );
			file.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );		
		}

		void testDo_WritesTheSameItWasRead()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetName( mPathToTestData + std::string( "Elvis.wav" ) );
			inputFile.RetrieveHeaderInfo();			

			CLAM::AudioFile outputFile;
			outputFile.AddAll();
			outputFile.UpdateData();
			outputFile.SetName( "CopyOfElvis.wav.wav" );
			outputFile.SetChannels( inputFile.GetChannels() );
			outputFile.SetSampleRate( inputFile.GetSampleRate() );
			outputFile.SetFormat( inputFile.GetFormat() );
			outputFile.SetEncoding( inputFile.GetEncoding() );
			outputFile.SetEndianess( inputFile.GetEndianess() );

			CLAM::MonoAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );
			cfgReader.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MonoAudioFileReader procReader;
			CLAM::MonoAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamples;
			readSamples.SetSize( 256 );

			(*(procReader.FirstOutput()))->Attach( readSamples );
			(*(procWriter.FirstInput()))->Attach( readSamples );

			procReader.Start();
			procWriter.Start();

			int  frameCounter = 0;

			while( procReader.Do() )
			{
				frameCounter++;
				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MonoAudioFileReader procReader2;
			inputFile.SetName( "CopyOfElvis.wav.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;
			readSamples2.SetSize( 256 );

			(*(procReader2.FirstOutput()))->Attach( readSamples2 );
			
			procReader.Start();
			procReader2.Start();

			frameCounter = 0;

			while( procReader.Do() && procReader2.Do() )
			{
				double sim = evaluateSimilarity( readSamples.GetBuffer(), readSamples2.GetBuffer() );

				CPPUNIT_ASSERT
					(  sim >= 0.9999 );

			}

			procReader.Stop();
			procReader2.Stop();		

		}

	};
}
