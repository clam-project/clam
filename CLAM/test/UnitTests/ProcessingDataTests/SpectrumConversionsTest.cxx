#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Spectrum.hxx"
#include "SpecTypeFlags.hxx"
#include "SpectrumConfig.hxx"

namespace CLAMTest
{

class SpectrumConversionsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SpectrumConversionsTest );

class SpectrumConversionsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectrumConversionsTest );
	CPPUNIT_TEST( testDefaultSpectrum );
	CPPUNIT_TEST( testMagPhase2BPF2MagPhase );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void testDefaultSpectrum()
	{
		CLAM::Spectrum spec;
		CPPUNIT_ASSERT( CLAM::TData(22050) == spec.GetSpectralRange() );
		CPPUNIT_ASSERT( 0 == spec.GetSize() );
		CPPUNIT_ASSERT( 0 == spec.GetBPFSize() );

		CLAM::SpecTypeFlags typeFlags;
		spec.GetType( typeFlags );
		CPPUNIT_ASSERT( true == typeFlags.bMagPhase );
		CPPUNIT_ASSERT( false == typeFlags.bComplex );
		CPPUNIT_ASSERT( false == typeFlags.bPolar );
		CPPUNIT_ASSERT( false == typeFlags.bMagPhaseBPF );

	}




	void testMagPhase2BPF2MagPhase()
	{
		CLAM::Spectrum spec;
		CLAM::SpectrumConfig config;
		config.SetSize(22050);
		spec.Configure(config);

		CLAM::DataArray & mag = spec.GetMagBuffer();
		CLAM::DataArray & phase = spec.GetPhaseBuffer();
		for(int i=0; i<22050; i++)
		{
			mag[i] = CLAM::TData(0.1);
			phase[i] = CLAM::TData(-0.1);
		}
		CLAM::SpecTypeFlags bpfAndMagphaseFlag;
		spec.SetType(bpfAndMagphaseFlag);

		CLAM::SpecTypeFlags magphaseFlag;
		magphaseFlag.bMagPhaseBPF = true;
		spec.SynchronizeTo(magphaseFlag);
		CPPUNIT_ASSERT_EQUAL(22050, spec.GetBPFSize() );

		CLAM::BPF & magBpf = spec.GetMagBPF();
		CLAM::BPF & phaseBpf = spec.GetPhaseBPF();
		for(int i=0; i<22050; i++)
		{
			CPPUNIT_ASSERT( CLAM::TData(0.1) == magBpf.GetXValue(i) );
			CPPUNIT_ASSERT( CLAM::TData(-0.1) == phaseBpf.GetXValue(i) );
		}

	}

	void testMagPhase2Complex2MagPhase()
	{

	}

	void testMagPhase2Polar2MagPhase()
	{

	}

	void testPolar2Complex2Polar()
	{

	}
};






} // namespace CLAMTest
