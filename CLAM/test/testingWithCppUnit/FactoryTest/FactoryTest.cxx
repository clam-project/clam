#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <typeinfo>

#include "Oscillator.hxx"

namespace CLAMTest
{

class FactoryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( FactoryTest );

class FactoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoryTest );
	CPPUNIT_TEST( testCreateOscillatorReturnsAnOscillator );

	CPPUNIT_TEST_SUITE_END();

private:
	
	void testCreateOscillatorReturnsAnOscillator() 
	{
	//	std::cout << typeid( 2 );
	//	std::cout << std::typeid( (CLAM::Oscillator*) 0 );

	}
};

	


} // namespace CLAMTest
