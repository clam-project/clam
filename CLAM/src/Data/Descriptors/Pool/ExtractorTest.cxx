#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"
#include "Pool.hxx"
#include "Spectrum.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{

class ExtractorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ExtractorTest );

class ExtractorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ExtractorTest );
	CPPUNIT_TEST(testTakeValue_whenString);
	CPPUNIT_TEST(testTakeValue_whenInteger);
	CPPUNIT_TEST(testTakeValue_whenAbstract);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testTakeValue_whenString()
	{
		const std::string expected = "ValueToExtract";
		CLAM::ReadBinder<std::string> binder;
		binder.BindTo(expected);
		const std::string & value = binder.TakeValue();
		CPPUNIT_ASSERT_EQUAL(expected, value);
	}

	void testTakeValue_whenInteger()
	{
		const int expected = 342;
		CLAM::ReadBinder<int> binder;
		binder.BindTo(expected);
		const int & value = binder.TakeValue();
		CPPUNIT_ASSERT_EQUAL(expected, value);
	}

	void testTakeValue_whenAbstract()
	{
		const std::string expected = "ValueToExtract";
		CLAM::ReadBinder<std::string> binder;
		binder.BindTo(expected);

		CLAM::AbstractReadBinder  & abstract = binder;
		const std::string & value = abstract.TakeValue<std::string>();
		CPPUNIT_ASSERT_EQUAL(expected, value);
	}

	
#ifdef NEVERDEFINED
	void testSubGoal()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddScope("AudioSample");
		scheme.AddAttribute<SignalLevel>   ("AudioSample","Level");
		scheme.AddScope("Frame");
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
