#include <cppunit/extensions/HelperMacros.h>
#include <typeinfo>
#include "cppUnitHelper.hxx"

#include "Oscillator.hxx"
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


	// Tests definition :
private:


	void testGetCreator_WhenIsEmpty()
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

	void testGetCreatorSafe_WhenIsEmpty()
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

	void testGetCreator_WrongKeyWithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		// set up:
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );

		CPPUNIT_ASSERT( NULL==reg.GetCreator("non existent key") );
	}

	void testGetCreatorSafe_WrongKeyWithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		// set up:
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );

		CPPUNIT_ASSERT( NULL==reg.GetCreator("non existent key") );
	}

	void testGetCreator_CorrectKeyWithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		// set up:
		CLAM::FactoryRegistry::CreatorMethod inserted;
		inserted = CLAM::CreateOscillator;

		reg.AddCreator( "Oscillator", inserted);
		CPPUNIT_ASSERT( inserted == reg.GetCreator("Oscillator") ); 
	}

	void testGetCreatorSafe_CorrectKeyWithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		// set up:
		CLAM::FactoryRegistry::CreatorMethod inserted;
		inserted = CLAM::CreateOscillator;

		reg.AddCreator( "Oscillator", inserted);
		CPPUNIT_ASSERT( inserted == reg.GetCreatorSafe("Oscillator") ); 
	}

	void testGetCreator_CorrectKeyWithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		// set up
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		reg.AddCreator( "AudioAdder", CLAM::CreateAudioAdder );

		CLAM::FactoryRegistry::CreatorMethod oscillatorCreator = CLAM::CreateOscillator;
		CPPUNIT_ASSERT( oscillatorCreator == reg.GetCreator("Oscillator") );
	}

	void testGetCreatorSafe_CorrectKeyWithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		// set up
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		reg.AddCreator( "AudioAdder", CLAM::CreateAudioAdder );

		CLAM::FactoryRegistry::CreatorMethod oscillatorCreator = CLAM::CreateOscillator;
		CPPUNIT_ASSERT( oscillatorCreator == reg.GetCreatorSafe("Oscillator") );
	}

	void testGetCreator_WrongKeyWithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		// set up
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		reg.AddCreator( "AudioAdder", CLAM::CreateAudioAdder );

		CPPUNIT_ASSERT( NULL == reg.GetCreator("Oscillator ") );
	}

	void testGetCreatorSafe_WrongKeyWithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		// set up
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		reg.AddCreator( "AudioAdder", CLAM::CreateAudioAdder );

		CPPUNIT_ASSERT( NULL == reg.GetCreatorSafe("incorrect as well") );
	}

	void testAddCreator_RepeatedKey()
	{
		CLAM::FactoryRegistry reg;
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );

		try {
			reg.AddCreator( "Oscillator", CLAM::CreateAudioAdder );
			CPPUNIT_FAIL( "Assert expected to happen" );

		} catch (CLAM::ErrAssertionFailed& ) {}
	}

	void testAddCreatorSafe_RepeatedKey()
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

	void testRemoveCreators_WhenIsEmpty()
	{
		CLAM::FactoryRegistry reg;
		reg.RemoveAllCreators();
		CPPUNIT_ASSERT_EQUAL_MESSAGE(
			"Count() should be 0 after RemoveAllCreators()",
			std::size_t(0), reg.Count() );
	}

	void testRemoveCreators_WhenNotEmtpy()
	{
		CLAM::FactoryRegistry reg;
		reg.AddCreator("osc", CLAM::CreateOscillator );
		reg.AddCreator("adder", CLAM::CreateAudioAdder );

		reg.RemoveAllCreators();
		CPPUNIT_ASSERT_EQUAL_MESSAGE(
			"Count() should be 0 after RemoveAllCreators()",
			std::size_t(0), reg.Count() );
	}

	void testCount_WhenEmpty()
	{
		CLAM::FactoryRegistry reg;
		CPPUNIT_ASSERT_EQUAL( std::size_t(0), reg.Count() );
	}

	void testCount_WithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		reg.AddCreator("osc", CLAM::CreateOscillator );
		reg.AddCreator("adder", CLAM::CreateAudioAdder );
		CPPUNIT_ASSERT_EQUAL( std::size_t(2), reg.Count() );
	}


};




} // namespace CLAMTest
