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
	CPPUNIT_TEST( testUnpopulatedScopeIsNotFreed );
	CPPUNIT_TEST( testPopulateScope_withANonExistingScope );
	CPPUNIT_TEST( testInstanciateAttribute_withinUnexistingScope );
	CPPUNIT_TEST( testInstanciateAttribute_withinUnpopulatedScope );
	CPPUNIT_TEST( testInstanciateAttribute_whenTheAttributeDoesNotExist );
	CPPUNIT_TEST( testInstanciateAttribute_whenTheAttributeDoesNotExistButTheScopeIsNotPopulated );
	CPPUNIT_TEST( testGetAttribute_withDifferentAttributes );
	CPPUNIT_TEST( testGetAttribute_withTheSameAttributeTwice );
	CPPUNIT_TEST( testGetAttribute_fromAUnpopulatedScope );
	CPPUNIT_TEST( testGetAttribute_fromUnexistingScope );
//	CPPUNIT_TEST( testGetAttribute_withNonInstantiatedAttribute );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute<CLAM::Attribute<CLAM::TData> >   ("AudioSample","Level");
		mScheme.AddAttribute<CLAM::Attribute<unsigned> >      ("Frame","Center");
		mScheme.AddAttribute<CLAM::Attribute<unsigned> >      ("Frame","Size");
		mScheme.AddAttribute<CLAM::Attribute<std::string> >   ("Frame","MyLabel");
	}

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	CLAM::DescriptionScheme mScheme;
private:
	void testUnpopulatedScopeIsNotFreed()
	{
		CLAM::DescriptionDataPool data(mScheme);
		// Should not be a memory leak
	}

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

	void testInstanciateAttribute_withinUnexistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("UnexistingScope","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "No scope registered with that name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstanciateAttribute_withinUnpopulatedScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("Frame","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Instantianting an attribute inside an unpopulated scope";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstanciateAttribute_whenTheAttributeDoesNotExist()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.PopulateScope("Frame",10);
		try
		{
			data.InstantiateAttribute("Frame","UnexistingAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Accessing an unexisting attribute inside a scope";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstanciateAttribute_whenTheAttributeDoesNotExistButTheScopeIsNotPopulated()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("Frame","UnexistingAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Accessing an unexisting attribute inside a scope";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetAttribute_withDifferentAttributes()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.PopulateScope("Frame",30);
		data.InstantiateAttribute("Frame","Center");
		data.InstantiateAttribute("Frame","Size");

		unsigned * centers = data.GetAttributePool<unsigned>("Frame","Center");
		unsigned * sizes   = data.GetAttributePool<unsigned>("Frame","Size");

		CPPUNIT_ASSERT(centers!=sizes);
	}

	void testGetAttribute_withTheSameAttributeTwice()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.PopulateScope("Frame",30);
		data.InstantiateAttribute("Frame","Center");

		unsigned * centers = data.GetAttributePool<unsigned>("Frame","Center");
		unsigned * centers2 = data.GetAttributePool<unsigned>("Frame","Center");

		CPPUNIT_ASSERT_EQUAL(centers,centers2);
	}

	void testGetAttribute_fromAUnpopulatedScope()
	{
		CLAM::DescriptionDataPool data(mScheme);

		try
		{
			unsigned * centers = data.GetAttributePool<unsigned>("Frame","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Accessing attribute data inside an unpopulated scope";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetAttribute_fromUnexistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);

		try
		{
			unsigned * centers = data.GetAttributePool<unsigned>("UnexistingScope","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "No scope registered with that name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	/*

	void testGetAttribute_withNonInstantiatedAttribute()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.PopulateScope("Frame",30);

		try
		{
			unsigned * centers = data.GetAttributePool<unsigned>("Frame","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Accessing a non instanciated attribute inside a scope";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}
	*/

};






} // namespace CLAMTest
