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
		CPPUNIT_TEST( testDo_PCM_WritesTheSameThatWasRead );
		CPPUNIT_TEST( testDo_OggVorbis_WritesTheSameThatWasRead );
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
			file.SetLocation( std::string( "NewFile.wav" ) );

			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );

		}

		void testDo_PCM_WritesTheSameThatWasRead()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "ElvisStereo.wav" ) );


			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "CopyOfElvisStereo.wav.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
						
			outputFileHeader.AddAll();
			outputFileHeader.UpdateData();

			outputFileHeader.SetChannels( inputFile.GetHeader().GetChannels() );
			outputFileHeader.SetSampleRate( inputFile.GetHeader().GetSampleRate() );
			outputFileHeader.SetFormat( inputFile.GetHeader().GetFormat() );
			outputFileHeader.SetEncoding( inputFile.GetHeader().GetEncoding() );
			outputFileHeader.SetEndianess( inputFile.GetHeader().GetEndianess() );

			outputFile.SetHeader( outputFileHeader );

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


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


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
			inputFile.SetLocation( "CopyOfElvisStereo.wav.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 256 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 256 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
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

		void testDo_OggVorbis_WritesTheSameThatWasRead()
		{
			
			CLAM::AudioFile inputFile;
			//inputFile.SetLocation( mPathToTestData + std::string( "ElvisStereo.wav" ) );
			inputFile.SetLocation( mPathToTestData + std::string( "JannieJones.ogg" ) );


			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "JannieJones-copy.ogg" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "VorbisMk1" );
	
			outputFile.SetHeader( outputFileHeader );

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
			readSamplesLeft.SetSize( 4096 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 4096 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

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
			inputFile.SetLocation( mPathToTestData + "JannieJones.ogg" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 4096 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 4096 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
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
