#include <cppunit/extensions/HelperMacros.h>
#include <typeinfo>
#include "cppUnitHelper.hxx"

#include "Oscillator.hxx"
#include "AudioAdder.hxx"

#include "Factory.hxx"

namespace CLAMTest
{

	

class FactoryRegistryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( FactoryRegistryTest );

class FactoryRegistryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoryRegistryTest );
	CPPUNIT_TEST( testGetCreator_WhenIsEmpty );
	CPPUNIT_TEST( testGetCreatorSafe_WhenIsEmpty );
	CPPUNIT_TEST( testGetCreator_WrongKeyWithASingleCreator );
	CPPUNIT_TEST( testGetCreatorSafe_WrongKeyWithASingleCreator );
	CPPUNIT_TEST( testGetCreator_CorrectKeyWithASingleCreator );
	CPPUNIT_TEST( testGetCreatorSafe_CorrectKeyWithASingleCreator );
	CPPUNIT_TEST( testGetCreator_CorrectKeyWithTwoCreators );
	CPPUNIT_TEST( testGetCreatorSafe_CorrectKeyWithTwoCreators );
	CPPUNIT_TEST( testAddCreator_RepeatedKey );
	CPPUNIT_TEST( testAddCreatorSafe_RepeatedKey );
	CPPUNIT_TEST( testRemoveCreators_WhenIsEmpty );
	CPPUNIT_TEST( testRemoveCreators_WhenNotEmtpy );
	CPPUNIT_TEST( testCount_WhenEmpty );
	CPPUNIT_TEST( testCount_WithTwoCreators );
	CPPUNIT_TEST_SUITE_END();


private:

	// helper methods:
	CLAM::Factory::CreatorMethod OscillatorCreator() { 
		return CLAM::Factory::Registrator<CLAM::Oscillator>::Create;
	}

	CLAM::Factory::CreatorMethod AudioAdderCreator() { 
		return CLAM::Factory::Registrator<CLAM::AudioAdder>::Create;
	}

	// tests definition
	void testGetCreator_WhenIsEmpty()
	{
		CLAM::Factory::Registry reg; // an empty factory register
		try {
			reg.GetCreator("bla");
			CPPUNIT_FAIL( "Assert was expected to happen");

		} catch( CLAM::ErrAssertionFailed& expected ) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string("the Factory Registry shouldn't be empty"),
				std::string( expected.what() ) );
		}
	}

	void testGetCreatorSafe_WhenIsEmpty()
	{
		CLAM::Factory::Registry reg; // an  empty factor y register
		try {
			reg.GetCreatorSafe("foo");
			CPPUNIT_FAIL( "it was expected to catch a CLAM::ErrFactory" );
		} catch (CLAM::ErrFactory& expected) {
			CPPUNIT_ASSERT_EQUAL_MESSAGE("In ErrFactory message:", 
				std::string("GetCreatorSafe invoked on an empty registry"), 
				std::string( expected.what() ) );
		}
	}

	void testGetCreator_WrongKeyWithASingleCreator()
	{
		CLAM::Factory::Registry reg;
		// set up:
		reg.AddCreator( "Oscillator", OscillatorCreator() );
		
		try{
			reg.GetCreator("non existent key");
			CPPUNIT_FAIL( "Assertion should happen" );
		} catch (CLAM::ErrAssertionFailed& ) {}
	}

	void testGetCreatorSafe_WrongKeyWithASingleCreator()
	{
		CLAM::Factory::Registry reg;
		// set up:
		reg.AddCreator( "Oscillator", OscillatorCreator() );
		
		try{
			reg.GetCreatorSafe("non existent key");
			CPPUNIT_FAIL( "ErrFactory expected" );
		} catch (CLAM::ErrFactory& ) {}
	}

	void testGetCreator_CorrectKeyWithASingleCreator()
	{
		CLAM::Factory::Registry reg;
		// set up:
		CLAM::Factory::CreatorMethod inserted;
		inserted = OscillatorCreator();

		reg.AddCreator( "Oscillator", inserted);
		CPPUNIT_ASSERT( inserted == reg.GetCreator("Oscillator") ); 
	}

	void testGetCreatorSafe_CorrectKeyWithASingleCreator()
	{
		CLAM::Factory::Registry reg;
		// set up:
		CLAM::Factory::CreatorMethod inserted;
		inserted = OscillatorCreator();

		reg.AddCreator( "Oscillator", inserted);
		CPPUNIT_ASSERT( inserted == reg.GetCreatorSafe("Oscillator") ); 
	}

	void testGetCreator_CorrectKeyWithTwoCreators()
	{
		CLAM::Factory::Registry reg;
		// set up
		reg.AddCreator( "Oscillator", OscillatorCreator() );
		reg.AddCreator( "AudioAdder", AudioAdderCreator() );

		CLAM::Factory::CreatorMethod oscillatorCreator = OscillatorCreator();
		CPPUNIT_ASSERT( oscillatorCreator == reg.GetCreator("Oscillator") );
	}

	void testGetCreatorSafe_CorrectKeyWithTwoCreators()
	{
		CLAM::Factory::Registry reg;
		// set up
		reg.AddCreator( "Oscillator", OscillatorCreator() );
		reg.AddCreator( "AudioAdder", AudioAdderCreator() );

		CLAM::Factory::CreatorMethod oscillatorCreator = OscillatorCreator();
		CPPUNIT_ASSERT( oscillatorCreator == reg.GetCreatorSafe("Oscillator") );
	}

	void testGetCreator_WrongKeyWithTwoCreators()
	{
		CLAM::Factory::Registry reg;
		// set up
		reg.AddCreator( "Oscillator", OscillatorCreator() );
		reg.AddCreator( "AudioAdder", AudioAdderCreator() );
		try{
			reg.GetCreator("wrong name");
			CPPUNIT_FAIL( "Assert expected to happen" );
		} catch (CLAM::ErrAssertionFailed& ) {}
	}

	void testGetCreatorSafe_WrongKeyWithTwoCreators()
	{
		CLAM::Factory::Registry reg;
		// set up
		reg.AddCreator( "Oscillator", OscillatorCreator() );
		reg.AddCreator( "AudioAdder", AudioAdderCreator() );
		
		try{
			reg.GetCreatorSafe("incorrect as well");
			CPPUNIT_FAIL( "CLAM::ErrFactory exptected" );
		} catch (CLAM::ErrFactory&) {}
	}

	void testAddCreator_RepeatedKey()
	{
		CLAM::Factory::Registry reg;
		reg.AddCreator( "Oscillator", OscillatorCreator() );

		try {
			reg.AddCreator( "Oscillator", AudioAdderCreator() );
			CPPUNIT_FAIL( "Assert expected to happen" );

		} catch (CLAM::ErrAssertionFailed& ) {}
	}

	void testAddCreatorSafe_RepeatedKey()
	{
		CLAM::Factory::Registry reg;
		reg.AddCreator( "Oscillator", OscillatorCreator() );

		try {
			reg.AddCreatorSafe( "Oscillator", AudioAdderCreator() );
			CPPUNIT_FAIL( "CLAM::ErrFactory expected" );

		} catch (CLAM::ErrFactory& expected) {
			CPPUNIT_ASSERT_EQUAL_MESSAGE("In ErrFactory message:", 
				std::string("A repeated key was passed"), 
				std::string( expected.what() ) );
		}
	}

	void testRemoveCreators_WhenIsEmpty()
	{
		CLAM::Factory::Registry reg;
		reg.RemoveAllCreators();
		CPPUNIT_ASSERT_EQUAL_MESSAGE(
			"Count() should be 0 after RemoveAllCreators()",
			std::size_t(0), reg.Count() );
	}

	void testRemoveCreators_WhenNotEmtpy()
	{
		CLAM::Factory::Registry reg;
		reg.AddCreator("osc", OscillatorCreator() );
		reg.AddCreator("adder", AudioAdderCreator() );

		reg.RemoveAllCreators();
		CPPUNIT_ASSERT_EQUAL_MESSAGE(
			"Count() should be 0 after RemoveAllCreators()",
			std::size_t(0), reg.Count() );
	}

	void testCount_WhenEmpty()
	{
		CLAM::Factory::Registry reg;
		CPPUNIT_ASSERT_EQUAL( std::size_t(0), reg.Count() );
	}

	void testCount_WithTwoCreators()
	{
		CLAM::Factory::Registry reg;
		reg.AddCreator("osc", OscillatorCreator() );
		reg.AddCreator("adder", AudioAdderCreator() );
		CPPUNIT_ASSERT_EQUAL( std::size_t(2), reg.Count() );
	}


};




} // namespace CLAMTest
