#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{


class ExtractorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ExtractorTest );

class ExtractorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ExtractorTest );
//	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute< CLAM::Attribute<char> >(
				"TestScope1","InputData");
		mScheme.AddAttribute< CLAM::Attribute<char> >(
				"TestScope1","OutputData");

		mPool = new CLAM::DescriptionDataPool(mScheme);
		mPool->SetNumberOfContexts("TestScope1",3);

		char * inputBuffer = mPool->GetAttributePool<char>("TestScope1","InputData");
		for (unsigned i = 0; i<3; i++)
			inputBuffer[i]='a'+i;

		mInputBuffer = inputBuffer;
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{ 
		delete mPool;
	}

private:
	CLAM::DescriptionScheme mScheme;
	CLAM::DescriptionDataPool * mPool;
	const char * mInputBuffer;
/*
	void test()
	{
		CPPUNIT_FAIL("Joder!!");
	}
*/

};


} // namespace CLAMTest
