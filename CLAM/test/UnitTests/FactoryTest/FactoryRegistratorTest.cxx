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

protected:
	typedef CLAM::Factory<CLAM::Processing> MyFactoryType;

private:
	void testCreate_ReturnsAnObjectOfTheTemplateType()
	{
		 MyFactoryType::AbstractProduct* created =
			 MyFactoryType::Registrator<CLAM::Oscillator>::Create();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created );
		delete created;
	}

	void testConstructorPassingKeyAndFactory_RegistersCreator()
	{
		MyFactoryType fact;
		const char* oscillatorClassName = "Oscilator";

		MyFactoryType::Registrator<CLAM::Oscillator> regt( oscillatorClassName, fact );

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

		MyFactoryType fact;
		// passing just the factory to the constructor
		MyFactoryType::Registrator<CLAM::Oscillator> regt( fact );

		MyFactoryType::AbstractProduct *created =
			fact.Create( oscillatorClassName );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created );
		delete created;
	}

	void testConstructorPassingKey_RegistersCreator()
	{
		MyFactoryType &theFactory = MyFactoryType::GetInstance();
		theFactory.Clear();

		CLAM::Oscillator dummy;
		MyFactoryType::Registrator<CLAM::Oscillator> regt( dummy.GetClassName() );

		MyFactoryType::AbstractProduct *created =
			theFactory.Create( dummy.GetClassName() );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created );

		// tear down
		delete created;
		theFactory.Clear();

	}

	void testDefaultConstructor_RegistersCreator()
	{
		MyFactoryType &theFactory = MyFactoryType::GetInstance();
		theFactory.Clear();

		MyFactoryType::Registrator<CLAM::Oscillator> DummyRegt;

		CLAM::Oscillator dummy;
		MyFactoryType::AbstractProduct *created =
			theFactory.Create( dummy.GetClassName() );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( CLAM::Oscillator, *created );

		// tear down
		delete created;
		theFactory.Clear();
	}
};

} // namespace CLAMTest
