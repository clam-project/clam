#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "MultiChannelAudioFileWriter.hxx"
#include "similarityHelper.hxx"

namespace CLAMTest
{
	class MultiChannelAudioFileWriterFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( MultiChannelAudioFileWriterFunctionalTest );

	class MultiChannelAudioFileWriterFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MultiChannelAudioFileWriterFunctionalTest );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_WithJustFilename );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_SampleRateNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_ChannelsNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_FormatNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_EncodingNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsTrue_EndianessNotPresent );
		CPPUNIT_TEST( testConfigure_ReturnsTrue_WithReasonableConfig );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_TooFewChannels );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_TooManyChannels );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_FormatAndEncodingIncompatibility );
		CPPUNIT_TEST( testDo_WritesTheSameThatWasRead );
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

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
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
			file.SetChannels( 2 );

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
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

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
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
			file.SetChannels( 2 );

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
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
			file.SetChannels( 2 );

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
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
			file.SetChannels( 2 );

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
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
			file.SetChannels( 2 );
			file.SetSampleRate( 22050 );
			file.SetFormat( CLAM::EAudioFileFormat::eWAV );
			file.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			file.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true,
					      configResult );
		}

		void  testConfigure_ReturnsFalse_TooFewChannels()
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

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );			
		}

		void testConfigure_ReturnsFalse_TooManyChannels()
		{
			CLAM::AudioFile file;
			file.AddAll();
			file.UpdateData();
			file.SetName( std::string( "NewFile.wav" ) );
			file.SetChannels( 32768 );
			file.SetSampleRate( 22050 );
			file.SetFormat( CLAM::EAudioFileFormat::eWAV );
			file.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			file.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
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
			file.SetChannels( 2 );
			file.SetSampleRate( 22050 );
			file.SetFormat( CLAM::EAudioFileFormat::eWAV );
			file.SetEncoding( CLAM::EAudioFileEncoding::eG723_24 );
			file.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );						
		}

		void testDo_WritesTheSameThatWasRead()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetName( mPathToTestData + std::string( "ElvisStereo.wav" ) );
			inputFile.RetrieveHeaderInfo();			

			CLAM::AudioFile outputFile;
			outputFile.AddAll();
			outputFile.UpdateData();
			outputFile.SetName( "CopyOfElvisStereo.wav.wav" );
			outputFile.SetChannels( inputFile.GetChannels() );
			outputFile.SetSampleRate( inputFile.GetSampleRate() );
			outputFile.SetFormat( inputFile.GetFormat() );
			outputFile.SetEncoding( inputFile.GetEncoding() );
			outputFile.SetEndianess( inputFile.GetEndianess() );

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			CLAM::Processing::OutPortIterator readerOutputIt = procReader.FirstOutput();
			CLAM::Processing::InPortIterator writerInputIt = procWriter.FirstInput();

			(*(readerOutputIt))->Attach( readSamplesLeft );
			readerOutputIt++;
			(*(readerOutputIt))->Attach( readSamplesRight );
			
			(*(writerInputIt))->Attach( readSamplesLeft );
			writerInputIt++;
			(*(writerInputIt))->Attach( readSamplesRight );

			procReader.Start();
			procWriter.Start();

			int  framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;
				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetName( "CopyOfElvisStereo.wav.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 256 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 256 );

			CLAM::Processing::OutPortIterator reader2OutIt =
				procReader2.FirstOutput();

			(*(reader2OutIt))->Attach( readSamplesLeft2 );
			reader2OutIt++;
			(*(reader2OutIt))->Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				CPPUNIT_ASSERT
					(  simLeft >= 0.9999 );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );
				CPPUNIT_ASSERT
					( simRight >= 0.9999 );

			}

			procReader.Stop();
			procReader2.Stop();		

			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			
		}
	};
}
