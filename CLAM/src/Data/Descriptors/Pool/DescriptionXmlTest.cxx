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
#include <sstream>


namespace CLAMTest
{

class DescriptionXmlTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DescriptionXmlTest );

class DescriptionXmlTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DescriptionXmlTest );
	CPPUNIT_TEST(testDumpScopePool_withNoAttributes);
	CPPUNIT_TEST(testDumpScopePool_withAttributesAndZeroSize);
	CPPUNIT_TEST(testDumpScopePool_withAttributes);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	std::ostringstream _targetStream;

	void testDumpAttributePool_withSimpleData()
	{
	}
	void testDumpAttributePool_withComponentData()
	{
	}
	void testDumpAttributePool_withNonDumpableData()
	{
	}
	void testDumpScopePool_withNoAttributes()
	{
		CLAM::DescriptionScope scope;
		CLAM::ScopePool pool(scope,20);

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool numberOfAttributes=\"0\" size=\"20\"/>"
			),_targetStream.str());
	}
	void testDumpScopePool_withAttributesAndZeroSize()
	{
		CLAM::DescriptionScope scope;
		scope.Add<std::string>("MyAttribute");
		CLAM::ScopePool pool(scope,0);

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool numberOfAttributes=\"1\" size=\"0\">"
			"<AttributePool name=\"MyAttribute\"/>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withAttributes()
	{
		CLAM::DescriptionScope scope;
		scope.Add<std::string>("MyAttribute");
		CLAM::ScopePool pool(scope,3);
		std::string * values = pool.GetWritePool<std::string>("MyAttribute");
		values[0]="value0";
		values[1]="value1";
		values[2]="value2";

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool numberOfAttributes=\"1\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"value0 value1 value2"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withNonInstantiatedAttributes()
	{
	}
	
#ifdef NEVERDEFINED
	const char * lala = 
		"<DescriptionDataPool NumberOfScopes='3'>"
		"<ScopePool name='Sample' size='2000'>"
			"<AttributePool name='Level' type='SignalLevel'>"
				"0.0 0.0 .342161 ...."
			"</AttributePool>"
		"</ScopePool>"
		"<ScopePool name='Frame' size='8'>"
			"<AttributePool name='Center' type='Reference:Sample'>"
				"0 256 512 768 ...."
			"</AttributePool>"
			"<AttributePool name='Center' type='SpectralDistribution'>"
				"<SpectralDistribution size='256'>"
					"0 0.7256 0.4512 0.8768 ...."
				"</SpectralDistribution>"
				"<SpectralDistribution size='256'>"
					"0 0.7256 0.4512 0.8768 ...."
				"</SpectralDistribution>"
				"<SpectralDistribution size='256'>"
					"0 0.7256 0.4512 0.8768 ...."
				"</SpectralDistribution>"
			"</AttributePool>"
		"</ScopePool>"
		"</DescriptionDataPool>";

	void testUsingThePoolDirectly()
	{
		typedef unsigned SamplePosition;
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::Attribute<CLAM::TData> >   ("AudioSample","Level");
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
					mean += (*frameAudio)*(*frameAudio);
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
#endif

};


} // namespace CLAMTest
