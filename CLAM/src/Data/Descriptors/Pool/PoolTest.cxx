#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{

class PoolTest;

CPPUNIT_TEST_SUITE_REGISTRATION( PoolTest );

class PoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PoolTest );
	CPPUNIT_TEST( testCalculationInserted );
	CPPUNIT_TEST( testCalculationCalculed );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void assertArrayEquals(unsigned size, CLAM::TData * expected, CLAM::TData * result)
	{
		for (unsigned int i = 0; i<size; i++)
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i],result[i],.0001);
	}

	void testCalculationInserted()
	{
		CLAM::ScopeSpec spec;
		CLAM::CalculationSpec calculation;
		spec.Add("Lala", calculation);

		CPPUNIT_ASSERT_EQUAL(std::string("Lala\n"),spec.DescriptorsList());
	}
	
	void testCalculationCalculed()
	{
		CLAM::TData data[10] = { 0,1,-2,3,-4,5,-6,7,-8,9 };
		CLAM::TData result[10] = { 4,4,4,4,4,4,4,4,4,4 };
		CLAM::TData expected[10] = { 0,1,2,3,4,5,6,7,8,9 };

//		CLAM::vectorize(CLAM::pow<n>)

	//	assertArraysEquals(10,expected,result);

	}
	
};






} // namespace CLAMTest
