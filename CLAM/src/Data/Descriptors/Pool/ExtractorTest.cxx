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

	CPPUNIT_TEST(testWriteInit_PointsToThePoolBegin);
	CPPUNIT_TEST(testWriteNext_PointsToTheNextPoolData);
	CPPUNIT_TEST(testWriteIsInsideScope_ReturnsTrueWhileInsideTheScope);
	CPPUNIT_TEST(testWriteIsInsideScope_ReturnsFalseBeyondTheScope);
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
		CLAM::Hook<CLAM::TData> hook;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		hook.Bind("TestScope1","TDataAttribute");
		hook.Init(*mPool);
		const CLAM::TData & result = hook.GetForReading();

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testNext_PointsToTheNextPoolData()
	{
		CLAM::Hook<CLAM::TData> hook;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		hook.Bind("TestScope1","TDataAttribute");
		hook.Init(*mPool);
		hook.Next();
		const CLAM::TData & result = hook.GetForReading();

		CPPUNIT_ASSERT_EQUAL(expected+1, &result);
	}

	void testIsInsideScope_ReturnsTrueWhileInsideTheScope()
	{
		CLAM::Hook<CLAM::TData> hook;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		hook.Bind("TestScope1","TDataAttribute");
		hook.Init(*mPool);

		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
	}

	void testIsInsideScope_ReturnsFalseBeyondTheScope()
	{
		CLAM::Hook<CLAM::TData> hook;
		const CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		hook.Bind("TestScope1","TDataAttribute");
		hook.Init(*mPool);

		// Advance until the end
		hook.Next();
		hook.Next();
		// Go Beyond
		hook.Next();

		CPPUNIT_ASSERT(!hook.IsInsideScope());
	}

	void testWriteInit_PointsToThePoolBegin()
	{
		CLAM::WriteHook<CLAM::TData> hook;
		hook.Bind("TestScope1","TDataAttribute");
		hook.Init(*mPool);
		CLAM::TData & result = hook.GetForWriting();

		CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testWriteNext_PointsToTheNextPoolData()
	{
		CLAM::WriteHook<CLAM::TData> hook;
		hook.Bind("TestScope1","TDataAttribute");
		hook.Init(*mPool);
		hook.Next();
		CLAM::TData & result = hook.GetForWriting();
		CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute") + 1;

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testWriteIsInsideScope_ReturnsTrueWhileInsideTheScope()
	{
		CLAM::WriteHook<CLAM::TData> hook;
		hook.Bind("TestScope1","TDataAttribute");
		hook.Init(*mPool);

		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
	}

	void testWriteIsInsideScope_ReturnsFalseBeyondTheScope()
	{
		CLAM::WriteHook<CLAM::TData> hook;
		CLAM::TData * expected = mPool->GetAttributePool<CLAM::TData>("TestScope1","TDataAttribute");
		hook.Bind("TestScope1","TDataAttribute");
		hook.Init(*mPool);

		// Advance until the end
		hook.Next();
		hook.Next();
		// Go Beyond
		hook.Next();

		CPPUNIT_ASSERT(!hook.IsInsideScope());
	}

#ifdef NEVERDEFINED
	void testSquareRootExtractor()
	{
		CLAM::SquareExtractor extractor(mScheme,"TestScope1");
		extractor.Hook("Squared").Attribute("TDataAttribute2");
		extractor.Hook("ToBeSquared").Attribute("TDataAttribute");
		for (extractor.Init(mPool);
			extractor.IsInsideScope();
			extractor.Next())
		{
			extractor.Extract();
		}
	}
#endif


};


} // namespace CLAMTest
