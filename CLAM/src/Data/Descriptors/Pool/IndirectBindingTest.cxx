#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"
#include "DataTypes.hxx"
#include "CharCopierExtractor.hxx"


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
	CPPUNIT_TEST(testExtraction_usingHooks);
	CPPUNIT_TEST(testExtraction_usingExtractor);

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
	void testExtraction_usingHooks()
	{
		CLAM::WriteHook<char> outputHook;
		CLAM::ReadIndirectHook<char> inputHook;
		inputHook.Bind("Referenced","Input");
		inputHook.Indirect("Referencer","Reference");
		outputHook.Bind("Referencer","Output");

		for (inputHook.Init(*mPool),outputHook.Init(*mPool); 
			inputHook.IsInsideScope() && outputHook.IsInsideScope();
			outputHook.Next(),inputHook.Next())
		{
			const char & input = inputHook.GetForReading();
			char & output = outputHook.GetForWriting();
			output = input;
		}
		std::string expected("adg",3);
		std::string result(mPool->GetAttributePool<char>("Referencer","Output"),3);
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

	void testExtraction_usingExtractor()
	{
		CharCopierExtractor extractor;

		CLAM::WriteHook<char> outputHook;
		CLAM::ReadIndirectHook<char> inputHook;
		inputHook.Bind("Referenced","Input");
		inputHook.Indirect("Referencer","Reference");
		outputHook.Bind("Referencer","Output");
		extractor.SetHooks(inputHook,outputHook);

		for (extractor.Init(*mPool); extractor.IsInsideScope(); extractor.Next())
		{
			extractor.Extract();
		}
		std::string expected("adg",3);
		std::string result(mPool->GetAttributePool<char>("Referencer","Output"),3);
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

};


} // namespace CLAMTest
