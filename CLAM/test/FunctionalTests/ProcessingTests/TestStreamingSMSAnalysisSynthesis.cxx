
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	


#include "SimpleOscillator.hxx"
#include "SMSAnalysisCore.hxx"
#include "SMSSynthesis.hxx"
#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"

namespace CLAMTest
{

class SMSSynthesisTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSSynthesisTest );

class SMSSynthesisTest : public CppUnit::TestFixture
{
public:
	
	CPPUNIT_TEST_SUITE( SMSSynthesisTest );
	
	CPPUNIT_TEST( testAnalysisSynthesis_doingStreaming );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		// instantiate processings: TODO
		ConfigureAnalysisSynthesis();
	}

private:

	CLAM::SMSAnalysisCore mAnalysis;
	CLAM::SMSSynthesis mSynthesis;

	void ConfigureAnalysisSynthesis()
	{
		/*global parameters*/
		int analWindowSize = 2049;
		int resAnalWindowSize = 1025;

		int analHopSize = 256;
//		analHopSize= (resAnalWindowSize-1)/2 ;

		int synthFrameSize = analHopSize;
		int analZeroPaddingFactor= 2;
		
		// SMS Analysis configuration 
		CLAM::SMSAnalysisConfig analConfig;
		
		analConfig.SetSinWindowSize(analWindowSize);
		analConfig.SetHopSize(analHopSize);
//		analConfig.SetSinWindowType(mGlobalConfig.GetAnalysisWindowType());
		analConfig.SetSinZeroPadding(analZeroPaddingFactor);
		analConfig.SetResWindowSize(resAnalWindowSize);
//		analConfig.SetResWindowType(mGlobalConfig.GetResAnalysisWindowType());

//		analConfig.GetPeakDetect().SetMagThreshold(mGlobalConfig.GetAnalysisPeakDetectMagThreshold());
//		analConfig.GetPeakDetect().SetMaxFreq(mGlobalConfig.GetAnalysisPeakDetectMaxFreq());
//		analConfig.GetSinTracking().SetIsHarmonic(mGlobalConfig.GetAnalysisHarmonic());
//		analConfig.GetFundFreqDetect().SetReferenceFundFreq(mGlobalConfig.GetAnalysisReferenceFundFreq());
//		analConfig.GetFundFreqDetect().SetLowestFundFreq(mGlobalConfig.GetAnalysisLowestFundFreq());
//		analConfig.GetFundFreqDetect().SetHighestFundFreq(mGlobalConfig.GetAnalysisHighestFundFreq());
		
		CLAM::SMSSynthesisConfig synthConfig;
		synthConfig.SetAnalWindowSize(resAnalWindowSize);
		synthConfig.SetFrameSize(synthFrameSize);
		synthConfig.SetHopSize(synthFrameSize);

//		mAnalysis.Configure(analConfig);	
//		mSynthesis.Configure(synthConfig);
		
	}


	void testAnalysisSynthesis_doingStreaming() //no segment, no frame 
	{
		CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;

		int frameSize = 256;
		CLAM::MonoAudioFileReader audioProvider;
		CLAM::MonoAudioFileReaderConfig readercfg;
		CLAM::MonoAudioFileWriter audioWriter;
		CLAM::MonoAudioFileWriterConfig writercfg;
		CLAM::AudioFile file;
		file.SetLocation("../../../../CLAM-TestData/sine.wav");
		readercfg.SetSourceFile(file);
		audioProvider.GetOutPort("Samples Read").SetSize( frameSize );
		audioProvider.GetOutPort("Samples Read").SetHop(frameSize);
		

		audioProvider.Configure( readercfg );
		file.SetLocation("../../../../CLAM-TestData/SMSTests/out_analysis-synthesis-streaming_sine.wav");
		CLAM::AudioFileHeader header;
		header.SetValues(44100, 1, "WAV");
		file.SetHeader( header );
		writercfg.AddTargetFile(); //TODO do the add at the DefaultInit
		writercfg.UpdateData();
		writercfg.SetTargetFile(file);
		audioWriter.Configure( writercfg );
		
//TODO new (alternative) interface:
//		CLAM::ConnectPorts(audioProvider, "Samples Read", mAnalysis, "Input Audio");

		audioProvider.GetOutPort("Samples Read").ConnectToIn( mAnalysis.GetInPort("Input Audio"));
		mAnalysis.GetOutPort("Sinusoidal Peaks").ConnectToIn( mSynthesis.GetInPort("InputSinPeaks"));
		mAnalysis.GetOutPort("Residual Spectrum").ConnectToIn( mSynthesis.GetInPort("InputResSpectrum"));
		mSynthesis.GetOutPort("OutputAudio").ConnectToIn( audioWriter.GetInPort("Samples Write") );

		audioProvider.Start();
		audioWriter.Start();
		mAnalysis.Start();
		mSynthesis.Start();

		// Processings firings
		CLAM_ASSERT(audioProvider.GetOutPort("Samples Read").CanProduce(), "mono audio file reader should have provided audio");
	
		while (audioProvider.Do())
		{
			mAnalysis.Do();
			if (mSynthesis.CanConsumeAndProduce())
			{
				mSynthesis.Do();
				audioWriter.Do();
			}
		}
		audioWriter.Stop();

					      
	}

	
};

} // namespace CLAMTest

