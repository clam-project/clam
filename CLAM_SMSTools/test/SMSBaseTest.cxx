#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

//#include "SMSBase.hxx"

namespace CLAMTest
{


class SMSExampleTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSExampleTest );

class SMSExampleTest : public CppUnit::TestFixture //, public SMSBase
{
	CPPUNIT_TEST_SUITE( SMSExampleTest );
	CPPUNIT_TEST( testSomeMethod_WhenSomeConditionsMeet );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void testSomeMethod_WhenSomeConditionsMeet()
	{
		// Setup

		// Exercise

		// Verification
		CPPUNIT_ASSERT_EQUAL( std::string("expected"), std::string("fail!") );
		
		// Tear down
	}
};






} // namespace CLAMTest
