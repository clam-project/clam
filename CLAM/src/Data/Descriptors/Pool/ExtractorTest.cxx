#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"


namespace CLAMTest
{

class ExtractorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ExtractorTest );

class ExtractorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ExtractorTest );
	CPPUNIT_TEST(testTakeValue_whenString);
	CPPUNIT_TEST(testTakeValue_whenInteger);
	CPPUNIT_TEST(testTakeValue_whenAbstract);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testTakeValue_whenString()
	{
		const std::string expected = "ValueToExtract";
		CLAM::ReadBinder<std::string> binder;
		binder.BindTo(expected);
		const std::string & value = binder.TakeValue();
		CPPUNIT_ASSERT_EQUAL(expected, value);
	}

	void testTakeValue_whenInteger()
	{
		const int expected = 342;
		CLAM::ReadBinder<int> binder;
		binder.BindTo(expected);
		const int & value = binder.TakeValue();
		CPPUNIT_ASSERT_EQUAL(expected, value);
	}

	void testTakeValue_whenAbstract()
	{
		const std::string expected = "ValueToExtract";
		CLAM::ReadBinder<std::string> binder;
		binder.BindTo(expected);

		CLAM::AbstractReadBinder  & abstract = binder;
		const std::string & value = abstract.TakeValue<std::string>();
		CPPUNIT_ASSERT_EQUAL(expected, value);
	}


};


} // namespace CLAMTest
