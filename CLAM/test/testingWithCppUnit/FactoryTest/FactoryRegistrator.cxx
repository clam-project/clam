#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Oscillator.hxx"
#include "Factory.hxx"

namespace CLAMTest
{

class FactoryRegistratorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( FactoryRegistratorTest );

class FactoryRegistratorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoryRegistratorTest );
	CPPUNIT_TEST( testCreate_ReturnsAnObjectOfTheTemplateType );
	CPPUNIT_TEST( testConstructorPassingAFactory_RegistersCreator );
	CPPUNIT_TEST_SUITE_END();

	void testCreate_ReturnsAnObjectOfTheTemplateType()
	{
		CLAM::Processing* created =
			CLAM::FactoryRegistrator<CLAM::Oscillator>::Create();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created ); 
		delete created;
	}

	void testConstructorPassingAFactory_RegistersCreator()
	{
		CLAM::Factory fact;
		CLAM::FactoryRegistrator<CLAM::Oscillator> foo( fact );
		
		const char* OscillatorClassName = "Oscillator";

		CLAM::Processing *created = 
			fact.Create( OscillatorClassName );
		
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created ); 
		delete created;	
	}
};

} // namespace CLAMTest



