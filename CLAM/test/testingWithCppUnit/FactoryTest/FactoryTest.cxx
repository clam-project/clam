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
	CPPUNIT_TEST( testAddCreator_WithRepeatedKey );
	CPPUNIT_TEST_SUITE_END();

protected:
	CLAM::Factory* _theFactory;

public:
	void setUp()
	{
		_theFactory = new CLAM::Factory;
	}
	void tearDown()
	{
		delete _theFactory;
	}

	// Tests definition :
protected:
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
		_theFactory->GetRegistry().AddCreator( "Oscillator", CLAM::CreateOscillator );
		
		CLAM::Processing* returned = _theFactory->MakeProcessing("Oscillator");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *returned );
		
		// tear down:
		delete returned;
		_theFactory->Clear();

	}

	void testMakeProcessingSafe_WithABadKey()
	{
		try{
			_theFactory->MakeProcessingSafe("Oscillator");
			CPPUNIT_FAIL("Should throw an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}


	void testAddCreator_WithRepeatedKey()
	{
		_theFactory->AddCreator("Oscillator", CLAM::CreateOscillator );
		try{
			_theFactory->AddCreator("Oscillator", CLAM::CreateOscillator);
			CPPUNIT_FAIL("an assertion should happen");
		} catch ( CLAM::ErrAssertionFailed& )
		{}
	}

	void testAddCreatorSafe_WithRepeatedKey()
	{
		_theFactory->AddCreator("Oscillator", CLAM::CreateAudioAdder );
		try{
			_theFactory->AddCreator("Oscillator", CLAM::CreateOscillator);
			CPPUNIT_FAIL("");
		} catch (CLAM::ErrFactory&) {
			
		}
	}

};



///////////////////////////////////////////////////////////////////////

class FactorySingletonTest;

CPPUNIT_TEST_SUITE_REGISTRATION( FactorySingletonTest );

class FactorySingletonTest : public FactoryTest
{
	CPPUNIT_TEST_SUITE( FactorySingletonTest );
	CPPUNIT_TEST( testCreateOscillatorReturnsAnOscillator );
	CPPUNIT_TEST( testMakeProcessing_ReturnsAnOscillator );
	CPPUNIT_TEST( testMakeProcessingSafe_WithABadKey );
	CPPUNIT_TEST( testFactoryIsSingleton );

	CPPUNIT_TEST_SUITE_END();

public:
	
	void setUp()
	{
		_theFactory = &CLAM::Factory::GetInstance();
	}

	void tearDown()
	{
		_theFactory->Clear();
	}

private:
	void testFactoryIsSingleton()
	{
		CLAM::Factory &ref1 = CLAM::Factory::GetInstance();
		CLAM::Factory &ref2 = CLAM::Factory::GetInstance();

		CPPUNIT_ASSERT_MESSAGE(
			"the thow Factory refs should point the same object ",
			&ref1 == &ref2);

	}
};

	


} // namespace CLAMTest
