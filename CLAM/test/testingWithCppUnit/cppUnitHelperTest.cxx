#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

namespace CLAMTest
{

class cppUnitHelperTest;
CPPUNIT_TEST_SUITE_REGISTRATION( cppUnitHelperTest );

class cppUnitHelperTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( cppUnitHelperTest );
	CPPUNIT_TEST( testAssertionTraitsTypeInfoToString );

	CPPUNIT_TEST_SUITE_END();

private:
	
	class Base { 
	public:
		virtual ~Base(){} 
	};
	class ConcreteFoo : public Base{};
	class ConcreteBar : public Base{};
	class NothingToDo {};
	void testAssertionTraitsTypeInfoToString()
	{
		
		Base* concrete = new ConcreteFoo;
		Base* base = new Base;

		std::string concreteStr = 
			CppUnit::assertion_traits<std::type_info>::toString( typeid(concrete) );
		
		std::string baseStr = 
			CppUnit::assertion_traits<std::type_info>::toString( typeid(base) );
				
		CPPUNIT_ASSERT_MESSAGE("typeid(...).name() of different classes should be differents",
			concreteStr != baseStr );
		

	}
	
};
} // namespace
 
/*
 template<>
 struct assertion_traits<std::string>   // specialization for the std::string type
 {
   static bool equal( const std::string& x, const std::string& y )
   {
     return x == y;
   }
 
   static std::string toString( const std::string& x )
   {
     std::string text = '"' + x + '"';    // adds quote around the string to see whitespace
     OStringStream ost;
     ost << text;
     return ost.str();
   }
 };
*/