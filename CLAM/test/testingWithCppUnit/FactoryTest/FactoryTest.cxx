#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

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
	CPPUNIT_TEST( testMakeProcessing_ReturnsAnOscillator );
	CPPUNIT_TEST( testMakeProcessingSafe_WithABadKey );
	CPPUNIT_TEST( testFactoryIsSingleton );
	CPPUNIT_TEST( testAutomaticRegistry );

	CPPUNIT_TEST_SUITE_END();

	

	// Tests definition :
private:
	
	void testCreateOscillatorReturnsAnOscillator() 
	{
		CLAM::Processing* returned = CLAM::CreateOscillator();
		
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *returned ); 
		delete returned;

		// test that we can store and postpone execution of such a creator method
		typedef CLAM::Processing* (*CreatorMethod)();
		CreatorMethod storedMethod = CLAM::CreateOscillator;
		returned = storedMethod();
		
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *returned );
		delete returned;
	}
	
	void testMakeProcessing_ReturnsAnOscillator()
	{
		// set up:
		CLAM::Factory &factory = CLAM::Factory::GetInstance();
		factory.Clear();
		factory.GetRegistry().AddCreator( "Oscillator", CLAM::CreateOscillator );
		
		CLAM::Processing* returned = factory.MakeProcessing("Oscillator");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *returned );
		
		// tear down:
		delete returned;
		factory.Clear();

	}

	void testMakeProcessingSafe_WithABadKey()
	{
		CLAM::Factory &factory = CLAM::Factory::GetInstance();
		try{
			factory.MakeProcessingSafe("Oscillator");
		} catch ( CLAM::ErrFactory& ) {}
	}

	void testFactoryIsSingleton()
	{
		// this is a compilation test (impossible to automate) :
		// discommenting the following lines must give a compiler error
	
		//CLAM::Factory fact; // error: ctr is private
		//delete &CLAM::Factory::GetInstance(); // error: dtr is private
		//class Sub : public CLAM::Factory {}; Sub s; // error: class is final


		CLAM::Factory &ref1 = CLAM::Factory::GetInstance();
		CLAM::Factory &ref2 = CLAM::Factory::GetInstance();

		CPPUNIT_ASSERT_MESSAGE(
			"the thow Factory refs should point the same object ",
			&ref1 == &ref2);

	}

	void testAutomaticRegistry()
	{
		CLAM::Factory &factory = CLAM::Factory::GetInstance();
		factory.Clear(); // make sure it's empty (although innecessary)
		
		// the ctr register the creator to the factory.
		CLAM::AutomaticRegistrator<CLAM::Oscillator> dummy;

		CLAM::Processing* returned = factory.MakeProcessing("Oscillator");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *returned );
		
		// tear down:
		delete returned;
		factory.Clear();

	}
};

	


} // namespace CLAMTest
