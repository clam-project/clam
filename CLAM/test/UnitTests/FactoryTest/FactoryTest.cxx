#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Oscillator.hxx"
#include "AudioAdder.hxx"

#include "Factory.hxx"


namespace CLAMTest
{


class FactoryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( FactoryTest );

class FactoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoryTest );
	CPPUNIT_TEST( testCreateOscillatorReturnsAnOscillator );
	CPPUNIT_TEST( testCreate_ReturnsAnOscillator );
	CPPUNIT_TEST( testCreateSafe_WithABadKey );
	CPPUNIT_TEST( testAddCreator_WithRepeatedKey );
	CPPUNIT_TEST( testAddCreatorSafe_WithRepeatedKey );

	CPPUNIT_TEST_SUITE_END();

protected:
	typedef CLAM::Factory<CLAM::Processing> MyFactoryType;

	MyFactoryType* _theFactory;

public:
	void setUp()
	{
		_theFactory = new MyFactoryType;
	}
	void tearDown()
	{
		delete _theFactory;
	}

	// helper methods:
	MyFactoryType::CreatorMethod OscillatorCreator() {
		return MyFactoryType::Registrator<CLAM::Oscillator>::Create;
	}

	MyFactoryType::CreatorMethod AudioAdderCreator() {
		return MyFactoryType::Registrator<CLAM::AudioAdder>::Create;
	}
	// Tests definition :
protected:
	void testCreateOscillatorReturnsAnOscillator()
	{
		CLAM::Processing* returned = OscillatorCreator()();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *returned );
		delete returned;
	}

	void testCreate_ReturnsAnOscillator()
	{
		_theFactory->AddCreator( "Oscillator", OscillatorCreator() );

		CLAM::Processing* returned = _theFactory->Create("Oscillator");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *returned );

		// tear down:
		delete returned;
		_theFactory->Clear();

	}

	void testCreateSafe_WithABadKey()
	{
		try{
			_theFactory->CreateSafe("Oscillator");
			CPPUNIT_FAIL("Should throw an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}


	void testAddCreator_WithRepeatedKey()
	{
		_theFactory->AddCreator("Oscillator", OscillatorCreator() );
		try{
			_theFactory->AddCreator("Oscillator", OscillatorCreator());
			CPPUNIT_FAIL("an assertion should happen");
		} catch ( CLAM::ErrAssertionFailed& )
		{}
	}

	void testAddCreatorSafe_WithRepeatedKey()
	{
		_theFactory->AddCreator("Oscillator", AudioAdderCreator() );
		try{
			_theFactory->AddCreatorSafe("Oscillator", OscillatorCreator());
			CPPUNIT_FAIL("an ErrFactory should be rised");
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
	CPPUNIT_TEST( testCreate_ReturnsAnOscillator );
	CPPUNIT_TEST( testCreateSafe_WithABadKey );
	CPPUNIT_TEST( testFactoryIsSingleton );

	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
		_theFactory = &MyFactoryType::GetInstance();
	}

	void tearDown()
	{
		_theFactory->Clear();
	}

private:
	void testFactoryIsSingleton()
	{
		MyFactoryType &ref1 = MyFactoryType::GetInstance();
		MyFactoryType &ref2 = MyFactoryType::GetInstance();

		CPPUNIT_ASSERT_MESSAGE(
			"the thow Factory refs should point the same object ",
			&ref1 == &ref2);

	}
};




} // namespace CLAMTest
