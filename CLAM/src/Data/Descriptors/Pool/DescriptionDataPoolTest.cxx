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
	CPPUNIT_TEST( testPopulateScope_withANonExistingScope );
	CPPUNIT_TEST( testPopulateScope_withAnExistingScope );
	CPPUNIT_TEST( testInstantiateAttribute_fromANonPopulatedScope );
	CPPUNIT_TEST( testInstantiateAttribute_fromANonExistingAttribute );
	CPPUNIT_TEST( testInstantiateAttribute_fromANonExistingScope );
	CPPUNIT_TEST( testInstantiateAttribute_withAnExistingScope );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute<CLAM::Attribute<CLAM::TData> >   ("AudioSample","Level");
		mScheme.AddAttribute<CLAM::Attribute<unsigned> >      ("Frame","Center");
		mScheme.AddAttribute<CLAM::Attribute<std::string> >   ("Frame","MyLabel");
	}

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	CLAM::DescriptionScheme mScheme;
private:
	void testPopulateScope_withANonExistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.PopulateScope("NonExistingScope",10);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "No scope registered with that name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testPopulateScope_withAnExistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.PopulateScope("AudioSample",10);
	}

	void testInstantiateAttribute_fromANonExistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("NonExistingScope","AnAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "No scope registered with that name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstantiateAttribute_fromANonExistingAttribute()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("AudioSample","ANonExistingAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "No scope registered with that name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstantiateAttribute_fromANonPopulatedScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("AudioSample","Level");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "No scope registered with that name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstantiateAttribute_withAnExistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.PopulateScope("AudioSample",10);
		data.InstantiateAttribute("AudioSample","Level");
		CLAM::TData * audio = data.GetAttributePool<CLAM::TData>("AudioSample","Level");
		// TODO: Which assert
	}
	
};






} // namespace CLAMTest
