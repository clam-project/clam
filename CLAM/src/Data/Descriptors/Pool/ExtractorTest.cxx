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
	CPPUNIT_TEST(testBinderInit_PointsToThePoolBegin);
	CPPUNIT_TEST(testBinderNext_PointsToTheNextPoolData);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{

		mScheme.AddAttribute< CLAM::Attribute<CLAM::TData> >(
				"TestScope1","TDataAttribute");

		mPool = new CLAM::DescriptionDataPool(mScheme);
		mPool->SetNumberOfContexts("TestScope1",3);
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{ 
		delete mPool;
	}

private:
	CLAM::DescriptionScheme mScheme;
	CLAM::DescriptionDataPool * mPool;

	void testBinderInit_PointsToThePoolBegin()
	{
		CLAM::Binder binder;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		binder.Init(*mPool, "TestScope1","TDataAttribute");
		const CLAM::TData & result = binder.GetForReading<CLAM::TData>();

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testBinderNext_PointsToTheNextPoolData()
	{
		CLAM::Binder binder;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		binder.Init(*mPool, "TestScope1","TDataAttribute");
		binder.Next();
		const CLAM::TData & result = binder.GetForReading<CLAM::TData>();

		CPPUNIT_ASSERT_EQUAL(expected+1, &result);
	}

};


} // namespace CLAMTest
