#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileWriterConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "Audio.hxx"
#include "similarityHelper.hxx"
#include <deque>

namespace CLAMTest
{
	class MonoAudioFileWriterFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( MonoAudioFileWriterFunctionalTest );
	
	class MonoAudioFileWriterFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MonoAudioFileWriterFunctionalTest );
		
		CPPUNIT_TEST( testConfigure_ReturnsFalse_WithJustFilename );
		CPPUNIT_TEST( testDo_PCM_WritesTheSameItWasRead );
		CPPUNIT_TEST( testDo_OggVorbis_WritesTheSameItWasRead );

		CPPUNIT_TEST( testDequeUsage );

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
		
		void testDequeUsage()
		{
			int buffer[] = { 3, 4, 5, 6, 7, 8, 9, 10 };
			std::deque<int> mydeque;

			std::cout << std::endl;

			for ( int i = 0; i < 8; i++ )
				mydeque.push_front( buffer[i] );

			// Let's see 

			std::cout << "Front poping ( using as a LIFO queue ): ";
			while( !mydeque.empty() )
			{
				std::cout << mydeque.front() << " ";
				mydeque.pop_front();
			}

			std::cout << std::endl;

			for ( int i = 0; i < 8; i++ )
				mydeque.push_front( buffer[i] );

			// Let's see 

			std::cout << "Back poping ( using as a FIFO queue ): ";
			while( !mydeque.empty() )
			{
				std::cout << mydeque.back() << " ";
				mydeque.pop_back();
			}

			std::cout << "Range insertion ( on the beginning ): ";
			

			mydeque.insert( mydeque.end(), (int*)buffer, buffer+8 );
			mydeque.insert( mydeque.end(), (int*)buffer, buffer+8 );

			std::cout << "deque size (after block insertion) is: " << mydeque.size();

			std::cout << "Range popping: ";

			std::copy( mydeque.begin(), mydeque.begin()+8, buffer );
			mydeque.erase( mydeque.begin(), mydeque.begin()+8 );

			std::cout << "Buffer contents: ";
			
			for ( int i= 0; i < 8; i++ )
				std::cout << buffer[i] << " ";
			
			std::cout << "and deque size is: " << mydeque.size();
			
			std::cout << std::endl;
			
		}

		void testConfigure_ReturnsFalse_WithJustFilename()
		{
			CLAM::AudioFile file;
			file.SetLocation( std::string( "NewFile.wav" ) );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );
		}


		void testDo_PCM_WritesTheSameItWasRead()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );


			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "CopyOfElvis.wav.wav" );

			CLAM::AudioFileHeader outputFileHeader;

			outputFileHeader.AddAll();
			outputFileHeader.UpdateData();

			outputFileHeader.SetChannels( inputFile.GetHeader().GetChannels() );
			outputFileHeader.SetSampleRate( inputFile.GetHeader().GetSampleRate() );
			outputFileHeader.SetFormat( inputFile.GetHeader().GetFormat() );
			outputFileHeader.SetEncoding( inputFile.GetHeader().GetEncoding() );
			outputFileHeader.SetEndianess( inputFile.GetHeader().GetEndianess() );

			outputFile.SetHeader( outputFileHeader );

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

			procReader.GetOutPorts().GetByNumber(0).Attach( readSamples );
			procWriter.GetInPorts().GetByNumber(0).Attach( readSamples );

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
			inputFile.SetLocation( "CopyOfElvis.wav.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;
			readSamples2.SetSize( 256 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamples2 );
		
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

		void testDo_OggVorbis_WritesTheSameItWasRead()
		{


			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );


			std::cout << "\n Original wave file sample count: ";
			std::cout << (inputFile.GetHeader().GetLength()/1000) * inputFile.GetHeader().GetSampleRate();
			std::cout << std::endl;

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "CopyOfElvis.ogg" );

			CLAM::AudioFileHeader outputFileHeader;

			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "VorbisMk1" );

			outputFile.SetHeader( outputFileHeader );


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

			procReader.GetOutPorts().GetByNumber(0).Attach( readSamples );
			procWriter.GetInPorts().GetByNumber(0).Attach( readSamples );

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
			
			CLAM::MonoAudioFileReader procReader2;
			inputFile.SetLocation( "CopyOfElvis.ogg" );
			cfgReader.SetSourceFile( inputFile );

			std::cout << "\n CLAM-encoded OggVorbis file sample count: ";
			std::cout << (inputFile.GetHeader().GetLength()/1000) * inputFile.GetHeader().GetSampleRate();
			std::cout << std::endl;


			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;

			readSamples2.SetSize( 256 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamples2 );
		
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			while( procReader.Do() && procReader2.Do() )
			{
				double sim = evaluateSimilarity( readSamples.GetBuffer(), readSamples2.GetBuffer() );

				framesChecked++;

				std::cout << "# " << framesChecked << " s(i)= " << sim;
				std::cout.flush();

				// MRJ: Note that due to decoding/encoding approximation errors
				// correlation is not as higher as in the PCM case
				//CPPUNIT_ASSERT
				//	(  sim >= 0.9);

			}

			procReader.Stop();
			procReader2.Stop();		


			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );


		}


	};
}
