#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Stats.hxx"

namespace CLAMTest
{



class StatsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( StatsTest );

class StatsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( StatsTest );
	CPPUNIT_TEST( testMoment1 );
	CPPUNIT_TEST( testMoment2 );
	CPPUNIT_TEST( testMoment3 );
	CPPUNIT_TEST( testMoment4 );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		_primeArray.AddElem(1.);
		_primeArray.AddElem(2.);
		_primeArray.AddElem(3.);
		_primeArray.AddElem(5.);
	}

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

		CLAM::Array<CLAM::TData> _primeArray;

	void testMoment1()
	{

		CLAM::Stats stats(&_primeArray);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(2.75), stats.GetMoment((CLAM::O<1>*)0));
	}
	void testMoment2()
	{
		CLAM::Stats stats(&_primeArray);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(9.75), stats.GetMoment((CLAM::O<2>*)0));
	}
	void testMoment3()
	{
		CLAM::Stats stats(&_primeArray);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(40.25), stats.GetMoment((CLAM::O<3>*)0));
	}
	void testMoment4()
	{
		CLAM::Stats stats(&_primeArray);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(180.75), stats.GetMoment((CLAM::O<4>*)0));
	}
};




} // namespace CLAMTes
