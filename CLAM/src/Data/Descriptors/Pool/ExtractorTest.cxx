#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{

class HookTest;

CPPUNIT_TEST_SUITE_REGISTRATION( HookTest );

class HookTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( HookTest );
	CPPUNIT_TEST(testInit_PointsToThePoolBegin);
	CPPUNIT_TEST(testNext_PointsToTheNextPoolData);
	CPPUNIT_TEST(testIsInsideScope_ReturnsTrueWhileInsideTheScope);
	CPPUNIT_TEST(testIsInsideScope_ReturnsFalseBeyondTheScope);
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

	void testInit_PointsToThePoolBegin()
	{
		CLAM::Hook<CLAM::TData> binder;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		binder.Init(*mPool, "TestScope1","TDataAttribute");
		const CLAM::TData & result = binder.GetForReading();

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testNext_PointsToTheNextPoolData()
	{
		CLAM::Hook<CLAM::TData> binder;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		binder.Init(*mPool, "TestScope1","TDataAttribute");
		binder.Next();
		const CLAM::TData & result = binder.GetForReading();

		CPPUNIT_ASSERT_EQUAL(expected+1, &result);
	}

	void testIsInsideScope_ReturnsTrueWhileInsideTheScope()
	{
		CLAM::Hook<CLAM::TData> binder;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		binder.Init(*mPool, "TestScope1","TDataAttribute");

		CPPUNIT_ASSERT(binder.IsInsideScope());
		binder.Next();
		CPPUNIT_ASSERT(binder.IsInsideScope());
		binder.Next();
		CPPUNIT_ASSERT(binder.IsInsideScope());
	}

	void testIsInsideScope_ReturnsFalseBeyondTheScope()
	{
		CLAM::Hook<CLAM::TData> binder;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		binder.Init(*mPool, "TestScope1","TDataAttribute");

		// Advance until the end
		binder.Next();
		binder.Next();
		// Go Beyond
		binder.Next();

		CPPUNIT_ASSERT(!binder.IsInsideScope());
	}

};


} // namespace CLAMTest
