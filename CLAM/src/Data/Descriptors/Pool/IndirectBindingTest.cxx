#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{


class IndirectBindingTest;

CPPUNIT_TEST_SUITE_REGISTRATION( IndirectBindingTest );

class IndirectBindingTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( IndirectBindingTest );
	CPPUNIT_TEST(testInit_PointsToThePlaceReferencedByTheFirstReference);
	CPPUNIT_TEST(testNext_PointsToThePlaceReferencedByTheSecondReference);
	CPPUNIT_TEST(testIsInsideScope_returnsTrueBeforeEnd);
	CPPUNIT_TEST(testIsInsideScope_returnsFalseAfterLastReference);
	CPPUNIT_TEST(testGetForReading_failsWhenInvalidReference);

	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute< CLAM::Attribute<char> >(
				"Referenced","Input");
		mScheme.AddAttribute< CLAM::Attribute<unsigned> >(
				"Referencer","BadReference");
		mScheme.AddAttribute< CLAM::Attribute<unsigned> >(
				"Referencer","Reference");
		mScheme.AddAttribute< CLAM::Attribute<char> >(
				"Referencer","Output");

		mPool = new CLAM::DescriptionDataPool(mScheme);
		mPool->SetNumberOfContexts("Referenced",10);
		mPool->SetNumberOfContexts("Referencer",3);
		{
			char * inputBuffer = mPool->GetAttributePool<char>("Referenced","Input");
			for (unsigned i = 0; i<10; i++)
				inputBuffer[i]='a'+i;
		}
		{
			unsigned * inputBuffer = mPool->GetAttributePool<unsigned>("Referencer","Reference");
			for (unsigned i = 0; i<3; i++)
				inputBuffer[i]=3*i;
		}
		{
			unsigned * inputBuffer = mPool->GetAttributePool<unsigned>("Referencer","BadReference");
			for (unsigned i = 0; i<3; i++)
				inputBuffer[i]=3*i;
			inputBuffer[0]=20;
		}

	}

	/// Common clean up, executed after each test method
	void tearDown()
	{ 
		delete mPool;
	}

private:
	CLAM::DescriptionScheme mScheme;
	CLAM::DescriptionDataPool * mPool;

	void testInit_PointsToThePlaceReferencedByTheFirstReference()
	{
		CLAM::ReadIndirectHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);
		const void * result = & (hook.GetForReading());
		const void * expected = mPool->GetReadAttributePool<char>("Referenced","Input");

		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

	void testNext_PointsToThePlaceReferencedByTheSecondReference()
	{
		CLAM::ReadIndirectHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);
		hook.Next();
		const void * result = & (hook.GetForReading());
		const void * thirdPosition =
			mPool->GetReadAttributePool<char>("Referenced","Input")+3;

		CPPUNIT_ASSERT_EQUAL(thirdPosition,result);
	}
	
	//  setUp reminder
	//  referenced :abcdefghij
	//              ^  ^  ^  
	//  referencer :036
	void testIsInsideScope_returnsTrueBeforeEnd()
	{
		CLAM::ReadIndirectHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);

		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
	}

	void testIsInsideScope_returnsFalseAfterLastReference()
	{
		CLAM::ReadIndirectHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);

		hook.Next();
		hook.Next();
		hook.Next();
		CPPUNIT_ASSERT(!hook.IsInsideScope());
	}

	void testGetForReading_failsWhenInvalidReference()
	{
		CLAM::ReadIndirectHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","BadReference");
		hook.Init(*mPool);
		try
		{
			const void * result = & (hook.GetForReading());
			CPPUNIT_FAIL("Should have failed an assertion");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Invalid cross-scope reference";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}


	}

	
/*
	void testInit_PointsToThePoolBegin()
	{
		const char * expected = mInputBuffer;
		CLAM::ReadHook<char> hook;
		hook.Bind("TestScope1","InputData");
		hook.Init(*mPool);
		const char & result = hook.GetForReading();

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testNext_PointsToTheNextPoolData()
	{
		const char * expected = mInputBuffer;
		CLAM::ReadHook<char> hook;
		hook.Bind("TestScope1","InputData");
		hook.Init(*mPool);
		hook.Next();
		const char & result = hook.GetForReading();

		CPPUNIT_ASSERT_EQUAL(expected+1, &result);
	}

	void testIsInsideScope_ReturnsTrueWhileInsideTheScope()
	{
		const char * expected = mInputBuffer;
		CLAM::ReadHook<char> hook;
		hook.Bind("TestScope1","InputData");
		hook.Init(*mPool);

		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
	}

	void testIsInsideScope_ReturnsFalseBeyondTheScope()
	{
		const char * expected = mInputBuffer;
		CLAM::ReadHook<char> hook;
		hook.Bind("TestScope1","InputData");
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
		CLAM::WriteHook<char> hook;
		hook.Bind("TestScope1","OutputData");
		hook.Init(*mPool);
		char & result = hook.GetForWriting();

		char * expected = mPool->GetAttributePool<char>("TestScope1","OutputData");

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testWriteNext_PointsToTheNextPoolData()
	{
		CLAM::WriteHook<char> hook;
		hook.Bind("TestScope1","OutputData");
		hook.Init(*mPool);
		hook.Next();
		char & result = hook.GetForWriting();
		char * expected = mPool->GetAttributePool<char>("TestScope1","OutputData") + 1;

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testWriteIsInsideScope_ReturnsTrueWhileInsideTheScope()
	{
		CLAM::WriteHook<char> hook;
		hook.Bind("TestScope1","OutputData");
		hook.Init(*mPool);

		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
	}

	void testWriteIsInsideScope_ReturnsFalseBeyondTheScope()
	{
		CLAM::WriteHook<char> hook;
		hook.Bind("TestScope1","OutputData");
		hook.Init(*mPool);

		// Advance until the end
		hook.Next();
		hook.Next();
		// Go Beyond
		hook.Next();

		CPPUNIT_ASSERT(!hook.IsInsideScope());
	}

	void testTransformUsingHooks()
	{
		CLAM::WriteHook<char> outputHook;
		CLAM::ReadHook<char> inputHook;
		inputHook.Bind("TestScope1","InputData");
		outputHook.Bind("TestScope1","OutputData");
		inputHook.Init(*mPool);
		outputHook.Init(*mPool);

		for (; inputHook.IsInsideScope() && outputHook.IsInsideScope(); outputHook.Next(), inputHook.Next())
		{
			char & output = outputHook.GetForWriting();
			const char  & input = inputHook.GetForReading();
			output = input;
		}
		
		std::string expected(mPool->GetAttributePool<char>("TestScope1","InputData"),3);
		std::string result(mPool->GetAttributePool<char>("TestScope1","OutputData"),3);
		

		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

	void testExtraction_usingExtractor()
	{
		CharCopierExtractor extractor;

		CLAM::WriteHook<char> outputHook;
		CLAM::ReadHook<char> inputHook;
		inputHook.Bind("TestScope1","InputData");
		outputHook.Bind("TestScope1","OutputData");
		extractor.SetHooks(inputHook,outputHook);

		for (extractor.Init(*mPool); extractor.IsInsideScope(); extractor.Next())
		{
			extractor.Extract();
		}
		
		std::string expected(mPool->GetAttributePool<char>("TestScope1","InputData"),3);
		std::string result(mPool->GetAttributePool<char>("TestScope1","OutputData"),3);
		
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
*/

};


} // namespace CLAMTest
