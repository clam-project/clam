#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"
#include "Pool.hxx"
#include "DataTypes.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "Audio.hxx"
#include "SpectralAnalysis.hxx"
#include "XMLStorage.hxx"


namespace CLAMTest
{

class ProofOfConceptTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ProofOfConceptTest );

class ProofOfConceptTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ProofOfConceptTest );
	CPPUNIT_TEST(testUsingThePoolDirectly);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testUsingThePoolDirectly()
	{
		typedef unsigned SamplePosition;
		CLAM::DescriptionScheme scheme;
//		scheme.AddScope("AudioSample");
		scheme.AddAttribute<CLAM::Attribute<CLAM::TData> >   ("AudioSample","Level");
//		scheme.AddScope("Frame");
		scheme.AddAttribute<CLAM::Attribute<SamplePosition> >      ("Frame","Center");
		scheme.AddAttribute<CLAM::Attribute<CLAM::TData> >         ("Frame","Energy");
		scheme.AddAttribute<CLAM::Attribute<CLAM::Spectrum> >      ("Frame","SpectralDistribution");
		
		CLAM::DescriptionDataPool pool(scheme);


		const unsigned audioSize = 3000;
		const unsigned frameSize = 256;
		pool.PopulateScope("AudioSample",audioSize);
		{
			CLAM::TData * audio = pool.GetAttributePool<CLAM::TData>("AudioSample","Level");
			for (unsigned i=0; i<audioSize; i++) audio[i] = 0;
			audio[0]=1.0;
			audio[audioSize-1]=2.0;
		}

		const unsigned nFrames = audioSize/frameSize;
		pool.PopulateScope("Frame",nFrames);
		{
			SamplePosition * centers = pool.GetAttributePool<SamplePosition>("Frame","Center");
			for (unsigned i=0; i<nFrames; i++)
			{
				centers[i] = i*frameSize;
			}
		}
		{
			CLAM::TData * means = pool.GetAttributePool<CLAM::TData>("Frame","Energy");
			const SamplePosition * centers = pool.GetReadAttributePool<SamplePosition>("Frame","Center");
			const CLAM::TData * samples = pool.GetReadAttributePool<CLAM::TData>("AudioSample","Level");
			for (unsigned i=0; i<nFrames; i++)
			{
				CLAM::TData & mean = means[i] = 0;
				const CLAM::TData * frameAudio = samples + centers[i];
				for (unsigned j=0; j<frameSize; frameAudio++, j++)
				{
					mean += (*frameAudio) * (*frameAudio);
				}
			}

		}
		{
			CLAM::Spectrum * spectrums = pool.GetAttributePool<CLAM::Spectrum>("Frame","SpectralDistribution");
			const SamplePosition * centers = pool.GetReadAttributePool<SamplePosition>("Frame","Center");
			const CLAM::TData * samples = pool.GetReadAttributePool<CLAM::TData>("AudioSample","Level");

			CLAM::SpecTypeFlags sflags;
			sflags.bMagPhase = true;
			sflags.bComplex = true;

			CLAM::SpectrumConfig     specconf;
			specconf.SetType( sflags );
			specconf.SetSpectralRange( 44100 / 2 );
			specconf.SetSize( 257 );

			CLAM::SpectralAnalysisConfig config;
			config.SetWindowSize(257);
			CLAM::SpectralAnalysis spectralAnalysis;
			spectralAnalysis.Start();
			for (unsigned i=0; i<nFrames; i++)
			{
				CLAM::Spectrum & spectrum = spectrums[i];
				CLAM::Audio audio;
				CLAM::DataArray & audioBuffer = const_cast<CLAM::DataArray & >(audio.GetBuffer());
				audioBuffer.SetPtr(const_cast<CLAM::TData *>(samples+centers[i]),frameSize);
				spectrum.Configure(specconf);
				spectralAnalysis.Do(audio, spectrum);
			}
			spectralAnalysis.Stop();
		}

//		CLAM::XmlStorage::Dump(pool, "Data", std::cout);
	}

	
#ifdef NEVERDEFINED
	void testSubGoal()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddScope("Frame");
		scheme.AddSignalScope("AudioSample");
		scheme.AddAttribute<SignalLevel>   ("AudioSample","Level");
		scheme.AddAttribute<SampleDuration>("Frame","Center");
		scheme.AddAttribute<Spectrum>      ("Frame","SpectralDistribution");

		CLAM::Scoper * loader = CLAM::Extractor::Create("SoundLoader");
		loader.BindOutputHook("AudioSample",);

		CLAM::Scoper * frametizer = CLAM::Extractor::Create("Frametizer");
		frametizer.BindOutputHook("Output","Frame");
		frametizer.BindOutputHook("Center","");
		frametizer.BindInputHook("Output")

		
		CLAM::Extractor * fft = CLAM::Extractor::Create("SpectralAnalysis");
		fft.BindOutputHook("Output","Frame","SpectralDistribution");
		fft.BindInputHook("Input",
			CurrentContext().Attribute("Center").Indirect("AudioSample")
				.RelativeRange(-framesize/2,+framesize/2).Attribute("Level")
			);

		scheme.AddExtractor(fft);
	}

	void testExtractionGoal()
	{
		std::string fileName("mysong.mpg");
		


		
	}

	void testGoal()
	{
		CLAM::DescriptionScheme scheme("DescriptionScheme.xml");
		scheme.AddPlugin("DescriptionSchemeExtension.xml");
		scheme.SetParameter("FrameSize",256);
		CLAM::DescriptionDataPool pool(scheme);
		pool.ExtractFrom("mysong.mp3");
		CLAM::XmlStorage::Dump(pool,"Description.xml","SimacDescription");
	}
#endif

};


} // namespace CLAMTest
