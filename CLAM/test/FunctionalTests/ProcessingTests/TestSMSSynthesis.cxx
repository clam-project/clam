
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	


/*

#include "AudioFile.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "Audio.hxx"
#include "Segment.hxx"
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
#include "AudioCollator.hxx"  // for helperFileExist

#include "Spectrum.hxx"
#include "SpectrumConfig.hxx" 
#include "Processing.hxx"

#include "OutPort.hxx"
#include "AudioOutPort.hxx"
*/


#include "SimpleOscillator.hxx"
#include "SMSAnalysisCore.hxx"
#include "SMSSynthesis.hxx"
#include "Audio.hxx"
#include "AudioOutPort.hxx"

namespace CLAMTest
{

class SMSSynthesisTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSSynthesisTest );

class SMSSynthesisTest : public CppUnit::TestFixture
{
public:
	
	CPPUNIT_TEST_SUITE( SMSSynthesisTest );
	
	CPPUNIT_TEST( testSynthesisOfOscillator );
	CPPUNIT_TEST_SUITE_END();

public: // TestFixture interface

	
	void testSynthesisOfOscillator()
	{
		CLAM::SimpleOscillator osc;
		CLAM::SMSAnalysisCore analysis;
		CLAM::SMSSynthesis synthesis;

		osc.GetOutPort("Audio Output").ConnectToIn( analysis.GetInPort("Input Audio"));

		analysis.GetOutPort("Sinusoidal Peaks").ConnectToIn( synthesis.GetInPort("InputSinPeaks"));
		analysis.GetOutPort("Residual Spectrum").ConnectToIn( synthesis.GetInPort("InputResSpectrum"));

		osc.Start();
		analysis.Start();
		synthesis.Start();

		osc.Do();
		analysis.Do();
		synthesis.Do();

		CLAM::OutPortBase & audioOut  = synthesis.GetOutPort("OutputAudio");
	//	CLAM::Audio & result = CLAM::AudioOutPort::GetLastWrittenAudio( audioOut );

	//	std::cout << "result size: " << result.GetSize() << std::endl;
		std::cout << "audio out size: " << audioOut.GetSize() << std::endl;
//		for(int i=0;i<result.GetSize(); i++)
//		{
//			std::cout << "index/out sample: " << i << "/" << result.GetBuffer()[i] << std::endl;
//		}
//		CPPUNIT_ASSERT_EQUAL( StringFromFile( mPathToTestData + "/SMSAnalysisTests/outputSpectrum_sine.xml" ), 
//	 			      StringFromData( CLAM::OutPortPublisher<CLAM::Spectrum>::GetLastWrittenData( outSpectrum)) );
					      
	}

	
};

} // namespace CLAMTest

