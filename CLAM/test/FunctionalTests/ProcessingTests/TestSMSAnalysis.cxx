
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	

#include "AudioFile.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "Audio.hxx"
#include "Segment.hxx"
//#include "SMSAnalysis.hxx"
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

#include "Spectrum.hxx"
#include "SpectrumConfig.hxx" 
#include "Processing.hxx"

namespace CLAMTest
{

class SMSAnalysisTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSAnalysisTest );

class SMSAnalysisTest : public CppUnit::TestFixture
{
public:
	
	CPPUNIT_TEST_SUITE( SMSAnalysisTest );

	CPPUNIT_TEST( testSine );
//	CPPUNIT_TEST( testTrumpet );
//	CPPUNIT_TEST( testSweep );
	
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
		CLAM::SpectralAnalysis sinSpectralAnalysis;
		CLAM::SpectralAnalysis resSpectralAnalysis;
		CLAM::SpectralPeakDetect peakDetect;
		CLAM::FundFreqDetect fundFreqDetect;
		CLAM::SinTracking sinTracking;
		CLAM::SynthSineSpectrum synthSineSpectrum;
		CLAM::SpectrumSubstracter2 spectrumSubstracter2;

		CLAM::TSize frameSize = 256;		
		CLAM::TSize sinWindowSize = 513;
		CLAM::TSize resWindowSize = 513;
		CLAM::TData samplingRate = 44100.0f;
		std::string audioFileName = mPathToTestData +  "sine.wav";

		CLAM::SpectrumConfig scfg;
		scfg.SetSize( resWindowSize/2+1);
		scfg.SetSpectralRange(samplingRate*0.5);
		CLAM::Spectrum resSpec;
		resSpec.Configure(scfg);
		scfg.SetSize(sinWindowSize/2+1);
		CLAM::Spectrum sinSpec;
		sinSpec.Configure(scfg);
	
		CLAM::SpectrumConfig Scfg; 
		CLAM::SpecTypeFlags sflags;
   		sflags.bComplex = 1;
 		sflags.bPolar = 0;
 		sflags.bMagPhase = 0;
 		sflags.bMagPhaseBPF = 0;
		Scfg.SetType(sflags);
	 	Scfg.SetSize(resSpec.GetSize());
 		Scfg.SetSpectralRange(samplingRate*0.5);
		CLAM::Spectrum tmpSpecIn(Scfg);    

		spectrumSubstracter2.SetPrototypes(resSpec,tmpSpecIn,resSpec);

		CLAM::AudioFile file;
		file.SetLocation( audioFileName );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader reader;
		reader.Configure( cfg );
	
		CLAM::TSize fileSize = file.GetHeader().GetSamples();
		
		// xr: todo it is done in the do()?
		//audio.SetSampleRate(file.GetHeader().GetSampleRate());

		reader.GetOutPort("Samples read").SetSize( frameSize );
		reader.GetOutPort("Samples read").SetHop( frameSize );
	
		sinSpectralAnalysis.GetInPort("Input").SetSize( sinWindowSize - 1 );
		sinSpectralAnalysis.GetInPort("Input").SetHop( frameSize );
		resSpectralAnalysis.GetInPort("Input").SetSize( resWindowSize - 1 );
		resSpectralAnalysis.GetInPort("Input").SetHop( frameSize );

		reader.GetOutPort("Samples read").ConnectToIn( sinSpectralAnalysis.GetInPort("Input"));
		reader.GetOutPort("Samples read").ConnectToIn( resSpectralAnalysis.GetInPort("Input"));
		
		sinSpectralAnalysis.GetOutPort("Output").ConnectToIn( peakDetect.GetInPort("Input spectrum"));
		peakDetect.GetOutPort( "Output spectral peak array" ).ConnectToIn( fundFreqDetect.GetInPort("Input"));
		peakDetect.GetOutPort( "Output spectral peak array" ).ConnectToIn( sinTracking.GetInPort( "Input" ));
		fundFreqDetect.GetOutControl( "Fund Freq Value").AddLink( &(sinTracking.GetInControl( "Fund Freq Value" )));
		sinTracking.GetOutPort( "Output").ConnectToIn( synthSineSpectrum.GetInPort( "Input" ));

		resSpectralAnalysis.GetOutPort("Output").ConnectToIn( spectrumSubstracter2.GetInPort( "Input 1"));
		synthSineSpectrum.GetOutPort("Output").ConnectToIn( spectrumSubstracter2.GetInPort( "Input 2"));
		
		//Read Audio File		
		reader.Start();	
		sinSpectralAnalysis.Start();
		peakDetect.Start();
		fundFreqDetect.Start();
		sinTracking.Start();
		synthSineSpectrum.Start();
		resSpectralAnalysis.Start();
		spectrumSubstracter2.Start();
		
		while( reader.Do() )
		{
			if( sinSpectralAnalysis.CanDoUsingPorts() && resSpectralAnalysis.CanDoUsingPorts() )
			{
				sinSpectralAnalysis.Do();
				peakDetect.Do();
				fundFreqDetect.Do();
				sinTracking.Do();
				synthSineSpectrum.Do();
				
				resSpectralAnalysis.Do();
				spectrumSubstracter2.Do();
			}
		}
		
		
		reader.Stop();
		sinSpectralAnalysis.Stop();
		peakDetect.Stop();
		fundFreqDetect.Stop();
		sinTracking.Stop();
		synthSineSpectrum.Stop();
		resSpectralAnalysis.Stop();
		spectrumSubstracter2.Stop();


/*		
		CLAM::Audio in;
		LoadAudioFile( in, mPathToTestData + std::string( "sine.wav" ) );
		CLAM::Segment segment;
		segment.SetAudio( in );
		SMSAnalysisWrapper analysis;
		analysis.Start();
		analysis.Do(segment);
		analysis.Stop();

		CPPUNIT_ASSERT_EQUAL( true, CheckResultWithStoredSegment( segment, mPathToTestData + "/SMSAnalysisTests/sine.xml" ) );
		*/
	}
/*
	void testSweep()
	{
		CLAM::Audio in;
		LoadAudioFile( in, mPathToTestData + std::string( "sweep.wav" ) );
		CLAM::Segment segment;
		segment.SetAudio( in );
		SMSAnalysisWrapper analysis;
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
		SMSAnalysisWrapper analysis;
		analysis.Start();
		analysis.Do(segment);
		analysis.Stop();

		CPPUNIT_ASSERT_EQUAL( true, CheckResultWithStoredSegment( segment, mPathToTestData + "/SMSAnalysisTests/trumpet.xml" ) );
	}
	*/

};

} // namespace CLAMTest

