#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

namespace CLAMTest
{

class cppUnitHelperTest;
CPPUNIT_TEST_SUITE_REGISTRATION( cppUnitHelperTest );

class cppUnitHelperTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( cppUnitHelperTest );
	CPPUNIT_TEST( testAssertionTraitsTypeInfoEqual_Different );

	CPPUNIT_TEST_SUITE_END();

private:
	
	class Base { virtual ~Base(){} };
	class ConcreteFoo {};
	class ConcreteBar {};
	class NothingToDo {};
	void testAssertionTraitsTypeInfoEqual_Different()
	{
	}
	
};
} // namespace
 