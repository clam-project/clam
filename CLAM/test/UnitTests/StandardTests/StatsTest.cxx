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
	CPPUNIT_TEST( testMoment5 );
	CPPUNIT_TEST( testMoment6 );
	CPPUNIT_TEST( testSlope_withPrimesUp );
	CPPUNIT_TEST( testSlope_withPrimesDown );
	CPPUNIT_TEST( testSlope_withConstant );
	CPPUNIT_TEST( testTilt_withPrimesUp );
	CPPUNIT_TEST( testTilt_withPrimesDown );
	CPPUNIT_TEST( testTilt_withConstant );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}
	
private:
	void fillPrimesUp()
	{
		_array.AddElem(1.);
		_array.AddElem(2.);
		_array.AddElem(3.);
		_array.AddElem(5.);
	}
	void fillConstant()
	{
		_array.AddElem(3.);
		_array.AddElem(3.);
		_array.AddElem(3.);
		_array.AddElem(3.);
	}

	void fillPrimesDown()
	{
		_array.AddElem(5.);
		_array.AddElem(3.);
		_array.AddElem(2.);
		_array.AddElem(1.);
	}

	CLAM::Array<CLAM::TData> _array;

private:
	void testMoment1()
	{
		fillPrimesUp();

		CLAM::Stats stats(&_array);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(2.75), stats.GetMoment((CLAM::O<1>*)0));
	}
	void testMoment2()
	{
		fillPrimesUp();

		CLAM::Stats stats(&_array);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(9.75), stats.GetMoment((CLAM::O<2>*)0));
	}
	void testMoment3()
	{
		fillPrimesUp();

		CLAM::Stats stats(&_array);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(40.25), stats.GetMoment((CLAM::O<3>*)0));
	}
	void testMoment4()
	{
		fillPrimesUp();

		CLAM::Stats stats(&_array);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(180.75), stats.GetMoment((CLAM::O<4>*)0));
	}
	void testMoment5()
	{
		fillPrimesUp();

		CLAM::Stats stats(&_array);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(850.25), stats.GetMoment((CLAM::O<5>*)0));
	}
	void testMoment6()
	{
		fillPrimesUp();

		CLAM::Stats stats(&_array);

		CPPUNIT_ASSERT_EQUAL( CLAM::TData(4104.75), stats.GetMoment((CLAM::O<6>*)0));
	}
	void testSlope_withPrimesUp()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(0.118182), stats.GetSlope() , .000001);
	}
	void testSlope_withPrimesDown()
	{
		fillPrimesDown();
		CLAM::Stats stats(&_array);

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(-0.118182), stats.GetSlope() , .000001);
	}
	void testSlope_withConstant()
	{
		fillConstant();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(0.), stats.GetSlope());
	}
	void testTilt_withPrimesUp()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(0.118182), stats.GetTilt() , .000001);
	}
	void testTilt_withPrimesDown()
	{
		fillPrimesDown();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(-0.118182), stats.GetTilt() , .000001);
	}
	void testTilt_withConstant()
	{
		fillConstant();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(0.), stats.GetTilt());
	}
};




} // namespace CLAMTes
