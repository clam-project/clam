#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for custom assert

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

	

	// Tests definition :
private:
	
	void testCreateOscillatorReturnsAnOscillator() 
	{
		CLAM::Processing* returned = CLAM::CreateOscillator();
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *returned ); 
	}
 

};

	


} // namespace CLAMTest
