#include <cppunit/extensions/HelperMacros.h>
#include "Processing.hxx"

namespace CLAMTest
{

class ProcessingTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ProcessingTest );

class ProcessingTest : public CppUnit::TestFixture, public CLAM::Processing
{
	CPPUNIT_TEST_SUITE( ProcessingTest );
	CPPUNIT_TEST( testGetInControl_GetTheRightControl );
	CPPUNIT_TEST( testGetOutControl_GetTheRightControl );
	CPPUNIT_TEST( testGetInControl_WithOutOfRangeIndexThrowException );
	CPPUNIT_TEST( testGetOutControl_WithOutOfRangeIndexThrowException );
	CPPUNIT_TEST( testLinkAndSendControl_ChangesInControlState );
	
	CPPUNIT_TEST_SUITE_END();

	// void implementation the pure virtual methods of processing
	ProcessingTest() : 
		_in("in",this), 
		_out("out",this), 
		_inTmpl("in",this,&ProcessingTest::ControlHandler)
	{};

	bool ConcreteConfigure(const CLAM::ProcessingConfig&) { return false; }
	const CLAM::ProcessingConfig &GetConfig() const {
		return *static_cast<CLAM::ProcessingConfig*>(0);
	}
	bool Do(void){ return false; }
	const char * GetClassName() const { return "ProcessingTest"; };	
	int ControlHandler( CLAM::TControlData ) { return 0; };
	
	CLAM::InControl _in;
	CLAM::InControlTmpl<ProcessingTest> _inTmpl;
	CLAM::OutControl _out;

	void testGetInControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("in"), GetInControl(0)->GetName() );
	}
	void testGetOutControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("out"), GetOutControl(0)->GetName() );
	}
	void testGetInControl_WithOutOfRangeIndexThrowException() 
	{
#		ifdef HAVE_STANDARD_VECTOR_AT // otherwise cannot be tested
			try
			{
				GetInControl(2); // we have two published in controls: indexs 0,1
				CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
			}
			catch( std::out_of_range&) {}
#		endif
	}
	void testGetOutControl_WithOutOfRangeIndexThrowException() 
	{
#		ifdef HAVE_STANDARD_VECTOR_AT // otherwise cannot be tested
			try
			{
				GetOutControl(1); // we have one published out controls: indexs 0
				CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
			}
			catch( std::out_of_range&) {}
#		endif
	}
	void testLinkAndSendControl_ChangesInControlState()
	{
		const int outId=0, inId=0;
		LinkOutWithInControl(outId, this, inId);
		SendControl(outId, 1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, GetInControl(inId)->GetLastValue() );
	}

};

} // namespace CLAMTest
