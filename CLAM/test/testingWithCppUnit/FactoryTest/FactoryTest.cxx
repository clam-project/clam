#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <typeinfo>

#include "Oscillator.hxx"
#include "Factory.hxx"

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
		CLAM::Processing* returned = CLAM::CreateOscillator();
		CPPUNIT_ASSERT( typeid( CLAM::Oscillator*) == typeid(returned)  ); 
	
	}
};

	


} // namespace CLAMTest
