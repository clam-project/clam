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
	CPPUNIT_TEST( testGetCreatorUnsafe_WhenIsEmpty );
	CPPUNIT_TEST( testGetCreatorSafe_WhenIsEmpty );
	CPPUNIT_TEST( testGetCreatorWrongKey_WithASingleCreator );
	CPPUNIT_TEST( testGetCreatorCorrectKey_WithASingleCreator );
	CPPUNIT_TEST( testGetCreatorCorrectKey_WithTwoCreators );
	CPPUNIT_TEST( testAddCreator_RepeatedKey );
	

	CPPUNIT_TEST_SUITE_END();

	

	// Tests definition :
private:
	

	void testGetCreatorUnsafe_WhenIsEmpty()
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

	void testGetCreatorWrongKey_WithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		// set up:
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		
		CPPUNIT_ASSERT( NULL==reg.GetCreator("non existent key") );
	}

	void testGetCreatorCorrectKey_WithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		// set up:
		CLAM::FactoryRegistry::CreatorMethod inserted;
		inserted = CLAM::CreateOscillator;

		reg.AddCreator( "Oscillator", inserted);

		CPPUNIT_ASSERT( inserted == reg.GetCreator("Oscillator") ); 
		CPPUNIT_ASSERT( inserted == reg.GetCreatorSafe("Oscillator") ); 
	}

	void testGetCreatorCorrectKey_WithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		// set up
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		reg.AddCreator( "AudioAdder", CLAM::CreateAudioAdder );

		CLAM::FactoryRegistry::CreatorMethod oscillatorCreator = CLAM::CreateOscillator;
		CPPUNIT_ASSERT( oscillatorCreator == reg.GetCreator("Oscillator") );
		CPPUNIT_ASSERT( oscillatorCreator == reg.GetCreatorSafe("Oscillator") );
	}

	void testGetCreatorIncorrectKey_WithTwoCreators()
	{
		CLAM::FactoryRegistry reg;
		// set up
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );
		reg.AddCreator( "AudioAdder", CLAM::CreateAudioAdder );

		CLAM::FactoryRegistry::CreatorMethod oscillatorCreator = CLAM::CreateOscillator;
		CPPUNIT_ASSERT( NULL == reg.GetCreator("Oscillator ") );
		CPPUNIT_ASSERT( NULL == reg.GetCreatorSafe("incorrect as well") );
	}

	void testAddCreator_RepeatedKey()
	{
		CLAM::FactoryRegistry reg;
		reg.AddCreator( "Oscillator", CLAM::CreateOscillator );

		try {
			reg.AddCreator( "Oscillator", CLAM::CreateAudioAdder );
			CPPUNIT_FAIL( "Assert expected to happen" );

		} catch (CLAM::ErrAssertionFailed& expected ) {}
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

};

	


} // namespace CLAMTest
