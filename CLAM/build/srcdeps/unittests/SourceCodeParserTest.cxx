#include <cppunit/extensions/HelperMacros.h>

namespace CLAMTest
{


class SourceCodeParserTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SourceCodeParserTest );

class SourceCodeParserTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SourceCodeParserTest );
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
		CPPUNIT_ASSERT_EQUAL( std::string("expected"), std::string("non expected") );
		
		// Tear down
	}
};






} // namespace CLAMTest