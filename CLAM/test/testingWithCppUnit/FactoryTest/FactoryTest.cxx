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
	CPPUNIT_TEST( testRegistryGetCreatorUnsafe_WhenIsEmpty );
	CPPUNIT_TEST( testRegistryGetCreatorSafe_WhenIsEmpty );
	CPPUNIT_TEST( testRegistryAskTheWrongKey_WithASingleCreator );
	CPPUNIT_TEST( testRegistryAskTheCorrectKey_WithASingleCreator );
	CPPUNIT_TEST( testRegistryAskACorrectKey_WithTwoCreators );
	CPPUNIT_TEST( testRegistryAddCreator_RepeatedKey );
	

	CPPUNIT_TEST_SUITE_END();

	

	// Tests definition :
private:
	
	void testCreateOscillatorReturnsAnOscillator() 
	{
		CLAM::Processing* returned = CLAM::CreateOscillator();
		std::string msg = std::string(
			"Expected: ") + typeid( CLAM::Oscillator ).name() +
			"But was: " + typeid( returned ).name();
		
		CPPUNIT_ASSERT_MESSAGE( msg, typeid( CLAM::Oscillator ) == typeid(*returned) ); 
	}
 
	void testRegistryGetCreatorUnsafe_WhenIsEmpty()
	{
		CLAM::FactoryRegistry reg; // an empty factory register
		try {
			reg.GetCreator("bla");
			CPPUNIT_FAIL( "Assert was expected to happen");

		} catch( CLAM::ErrAssertionFailed& expected ) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string("the Factory Registry shouldn't be empty"),
				std::string( expected.what() ) );
		}
	}

	void testRegistryGetCreatorSafe_WhenIsEmpty()
	{
		CLAM::FactoryRegistry reg; // an  empty factor y register
		try {
			reg.GetCreatorSafe("foo");
			CPPUNIT_FAIL( "it was expected to catch a CLAM::ErrFactory" );
		} catch (CLAM::ErrFactory& expected) {
			CPPUNIT_ASSERT_EQUAL_MESSAGE("In ErrFactory message:", 
				std::string("GetCreatorSafe invoked on an empty registry"), 
				std::string( expected.what() ) );
		}
	}

	void testRegistryAskTheWrongKey_WithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		// set up:
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		
		CPPUNIT_ASSERT( NULL==reg.GetCreator("non existent key") );
	}

	void testRegistryAskTheCorrectKey_WithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		// set up:
		CLAM::FactoryRegistry::CreatorMethod inserted;
		inserted = CLAM::CreateOscillator;

		reg.AddCreator( "Oscillator", inserted);

		CPPUNIT_ASSERT( inserted == reg.GetCreator("Oscillator") ); 
		CPPUNIT_ASSERT( inserted == reg.GetCreatorSafe("Oscillator") ); 
	}

	void testRegistryAskACorrectKey_WithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		// set up
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		reg.AddCreator( "AudioAdder", CLAM::CreateAudioAdder );

		CLAM::FactoryRegistry::CreatorMethod oscillatorCreator = CLAM::CreateOscillator;
		CPPUNIT_ASSERT( oscillatorCreator == reg.GetCreator("Oscillator") );
		CPPUNIT_ASSERT( oscillatorCreator == reg.GetCreatorSafe("Oscillator") );
	}

	void testRegistryAskIncorrectKey_WithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		// set up
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		reg.AddCreator( "AudioAdder", CLAM::CreateAudioAdder );

		CLAM::FactoryRegistry::CreatorMethod oscillatorCreator = CLAM::CreateOscillator;
		CPPUNIT_ASSERT( NULL == reg.GetCreator("Oscillator ") );
		CPPUNIT_ASSERT( NULL == reg.GetCreatorSafe("incorrect as well") );
	}

	void testRegistryAddCreator_RepeatedKey()
	{
		CLAM::FactoryRegistry reg;
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );

		try {
			reg.AddCreator( "Oscillator", CLAM::CreateAudioAdder );
			CPPUNIT_FAIL( "Assert expected to happen" );

		} catch (CLAM::ErrAssertionFailed& expected ) {}
	}

	void testRegistryAddCreatorSafe_RepeatedKey()
	{
		CLAM::FactoryRegistry reg;
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );

		try {
			reg.AddCreatorSafe( "Oscillator", CLAM::CreateAudioAdder );
			CPPUNIT_FAIL( "CLAM::ErrFactory expected" );

		} catch (CLAM::ErrFactory& expected) {
			CPPUNIT_ASSERT_EQUAL_MESSAGE("In ErrFactory message:", 
				std::string("FactoryRegistry::AddCreatorSafe(...) a repeated key was passed"), 
				std::string( expected.what() ) );
		}
	}

};

	


} // namespace CLAMTest
