#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{

CLAM::TData testingDoubler(CLAM::TData)
{
}

class PoolTest;

CPPUNIT_TEST_SUITE_REGISTRATION( PoolTest );

class PoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PoolTest );
	CPPUNIT_TEST( testCalculationInserted );
	CPPUNIT_TEST( testCalculationCalculed );
	CPPUNIT_TEST( testApplyUnaryFunctionAbs );
	CPPUNIT_TEST( testApplyUnaryFunctionNeg );
	CPPUNIT_TEST( testApplyUnaryComposition );
	CPPUNIT_TEST( testApplyUnaryCompositionInverted );
	CPPUNIT_TEST( testComposingWithApplyOperator );
	CPPUNIT_TEST( testComposingWithApplyOperatorInverted );
	CPPUNIT_TEST( testComposingCompositions );
	CPPUNIT_TEST( testPows );
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
	
	void testApplyUnaryFunctionAbs()
	{
		CLAM::abs myAbs;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myAbs(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myAbs(2.0),.0001);
	}

	void testApplyUnaryFunctionNeg()
	{
		CLAM::neg myNeg;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myNeg(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, myNeg(2.0),.0001);
	}

	void testApplyUnaryComposition()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, CLAM::compose (myNeg,myAbs)(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, CLAM::compose (myNeg,myAbs)(2.0),.0001);
	}

	void testApplyUnaryCompositionInverted()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, CLAM::compose (myAbs,myNeg)(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, CLAM::compose (myAbs,myNeg)(2.0),.0001);
	}

	void testComposingWithApplyOperator()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, myNeg(myAbs)(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, myNeg(myAbs)(2.0),.0001);
	}

	void testComposingWithApplyOperatorInverted()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myAbs(myNeg)(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myAbs(myNeg)(2.0),.0001);
	}

	void testComposingCompositions()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;
		CLAM::cubed myCubed;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(-8, myCubed(myNeg(myAbs))(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-8, myCubed(myNeg(myAbs))(2.0),.0001);
	}

	void testPows()
	{
		CLAM::pow<0> pow0;
		CLAM::pow<1> identity;
		CLAM::pow<2> squared;
		CLAM::pow<3> cubed;
		CLAM::pow<4> pow4;
		CLAM::pow<5> pow5;
		CLAM::pow<6> pow6;
		CLAM::pow<7> pow7;
		CLAM::pow<8> pow8;
		CLAM::pow<9> pow9;
		CLAM::pow<37> pow37;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  1, pow0(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  2, identity(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  4, squared(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  8, cubed(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 16, pow4(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 32, pow5(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 64, pow6(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(128, pow7(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(256, pow8(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(512, pow9(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(137438953472., pow37(2.0),.0001);
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
