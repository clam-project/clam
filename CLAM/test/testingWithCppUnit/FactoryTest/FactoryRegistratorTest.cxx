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
	CPPUNIT_TEST( testConstructorPassingKeyAndFactory_RegistersCreator );
	CPPUNIT_TEST( testConstructorPassingFactory_RegistersCreator );
	CPPUNIT_TEST( testConstructorPassingKey_RegistersCreator );
	CPPUNIT_TEST( testDefaultConstructor_RegistersCreator );
	CPPUNIT_TEST_SUITE_END();

private:
	void testCreate_ReturnsAnObjectOfTheTemplateType()
	{
		CLAM::Processing* created =
			CLAM::Factory::Registrator<CLAM::Oscillator>::Create();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created ); 
		delete created;
	}

	void testConstructorPassingKeyAndFactory_RegistersCreator()
	{
		CLAM::Factory fact;
		const char* oscillatorClassName = "Oscilator";
		
		CLAM::Factory::Registrator<CLAM::Oscillator> regt( oscillatorClassName, fact );
		
		CLAM::Processing *created = 
			fact.Create( oscillatorClassName );
		
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created ); 
		delete created;	
	}

	void testConstructorPassingFactory_RegistersCreator()
	{
		CLAM::Oscillator dummy; //needed for calling GetClassName()
		// here we use the name just for creating. Not for registrating
		const char* oscillatorClassName = dummy.GetClassName();
		
		CLAM::Factory fact;	
		// passing just the factory to the constructor
		CLAM::Factory::Registrator<CLAM::Oscillator> regt( fact );
			
		CLAM::Processing *created = 
			fact.Create( oscillatorClassName );
		
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created ); 
		delete created;	
	}

	void testConstructorPassingKey_RegistersCreator()
	{
		CLAM::Factory &theFactory = CLAM::Factory::GetInstance();
		theFactory.Clear();

		CLAM::Oscillator dummy;
		CLAM::Factory::Registrator<CLAM::Oscillator> regt( dummy.GetClassName() );
		
		CLAM::Processing *created = 
			theFactory.Create( dummy.GetClassName() );
		
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created );
		
		// tear down
		delete created;
		theFactory.Clear();

	}

	void testDefaultConstructor_RegistersCreator()
	{
		CLAM::Factory &theFactory = CLAM::Factory::GetInstance();
		theFactory.Clear();

		CLAM::Factory::Registrator<CLAM::Oscillator> DummyRegt;
		
		CLAM::Oscillator dummy;
		CLAM::Processing *created = 
			theFactory.Create( dummy.GetClassName() );
		
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created );
		
		// tear down
		delete created;
		theFactory.Clear();
	}
};

} // namespace CLAMTest
