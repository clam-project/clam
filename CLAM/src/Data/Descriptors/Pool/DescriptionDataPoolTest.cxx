#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"
#include "Component.hxx"
#include "Spectrum.hxx"


namespace CLAMTest
{

class DescriptionDataPoolTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DescriptionDataPoolTest );

class DescriptionDataPoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DescriptionDataPoolTest );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute<CLAM::Attribute<CLAM::TData> >   ("AudioSample","Level");
		mScheme.AddAttribute<CLAM::Attribute<unsigned> >      ("Frame","Center");
		mScheme.AddAttribute<CLAM::Attribute<CLAM::TData> >   ("Frame","Energy");
	}

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	CLAM::DescriptionScheme mScheme;
private:
	void testDescriptionScheme_withNoScopeRegistered()
	{
		CLAM::DescriptionScheme scheme;
		try
		{
			scheme.GetSpec("NonExistent");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "No scope registered with that name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testDescriptionScheme_withARegisteredAttribute()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute< CLAM::Attribute<CLAM::TData> >("MyScope","MyAttribute");
		scheme.AddAttribute< CLAM::Attribute<CLAM::TData> >("MyScope","MyOtherAttribute");

		const CLAM::DescriptionScope & spec = scheme.GetSpec("MyScope");

		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("MyOtherAttribute"));
	}

	void testDescriptionScheme_withTwoScopes()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute< CLAM::Attribute<CLAM::TData> >("MyScope","MyAttribute");
		scheme.AddAttribute< CLAM::Attribute<CLAM::TData> >("YourScope","YourAttribute");
		scheme.AddAttribute< CLAM::Attribute<int> >("YourScope","YourIntAttribute");

		const CLAM::DescriptionScope & mySpec = scheme.GetSpec("MyScope");
		const CLAM::DescriptionScope & yourSpec = scheme.GetSpec("YourScope");

		CPPUNIT_ASSERT_EQUAL(0u,mySpec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(0u,yourSpec.GetIndex("YourAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,yourSpec.GetIndex("YourIntAttribute"));
	}

};






} // namespace CLAMTest
