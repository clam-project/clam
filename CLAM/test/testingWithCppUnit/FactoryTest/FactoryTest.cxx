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
	

	CPPUNIT_TEST_SUITE_END();

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

		} catch( CLAM::ErrAssertionFailed& e ) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string("the Factory Registry shouldn't be empty"),
				std::string( e.what() ) );
			return;
		}
		CPPUNIT_FAIL( "Assert was expected to happen");
	}

	void testRegistryGetCreatorSafe_WhenIsEmpty()
	{
		CLAM::FactoryRegistry reg; // an  empty factor y register
		try {
			reg.GetCreatorSafe("foo");
		} catch (CLAM::ErrFactory e) {
			CPPUNIT_ASSERT_EQUAL_MESSAGE("In ErrFactory message:", 
				std::string("GetCreatorSafe invoked on an empty registry"), 
				std::string( e.what() ) );
			return;
		}
		CPPUNIT_FAIL( "it was extected to catch a CLAM::ErrFactory" );
	}

	void testRegistryAskTheWrongKey_WithASingleCreator()
	{
		CLAM::FactoryRegistry reg;
		
		CPPUNIT_ASSERT_EQUAL(NULL, reg.GetCreator("non existent key") );
	}
};

	


} // namespace CLAMTest
