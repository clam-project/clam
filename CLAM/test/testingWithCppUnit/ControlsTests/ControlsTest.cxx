/*
* Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
*                         UNIVERSITAT POMPEU FABRA
*
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#include "InControl.hxx"
#include "Processing.hxx"
#include "ProcessingComposite.hxx"

#include <cppunit/extensions/HelperMacros.h>
#include <sstream>

namespace CLAMTest {

using CLAM::TControlData;

class ControlsTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ControlsTest );

class ControlsTest : public CppUnit::TestFixture
{
	// old tests (to remove) :
	CPPUNIT_TEST_SUITE( ControlsTest );
	CPPUNIT_TEST( ProcessingSideInterface );
	CPPUNIT_TEST_EXCEPTION( ProcessingSidePublishedContainers, std::out_of_range );

	// new tests
	CPPUNIT_TEST( testInControl_DoControl_ChangesInternalState );
	CPPUNIT_TEST( testLinkAndSendControl_ChangesInControlInternalState );
	CPPUNIT_TEST( testInControlTmpl_DoControl_ChangesInternalState );
	CPPUNIT_TEST( testLinkAndSendWithInControlTmpl_CallbackMethodGetsCalled );
	CPPUNIT_TEST( testLinkAndSendWithInControlTmpl_CallbackWithIdMethodGetsCalled );
	CPPUNIT_TEST( testInControl_GetName_ChangesInteralState );
	CPPUNIT_TEST( testOutControl_GetName_ChangesInteralState );
	CPPUNIT_TEST_SUITE_END();
	
	std::stringstream _ost;
private:
	// old tests

	void ProcessingSidePublishedContainers();
	void ProcessingSideInterface();

	// new tests
	void testInControl_DoControl_ChangesInternalState()
	{
		CLAM::InControl in("i'm an in control");
		in.DoControl(1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, in.GetLastValue() );
	}

	void testLinkAndSendControl_ChangesInControlInternalState()
	{
		CLAM::InControl in("in");
		CLAM::OutControl out("out");
		out.AddLink(&in);
		out.SendControl(1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f , in.GetLastValue() );
	}
	// this method is used by the CLAM::InControlTmpl<T>
	// here we are simulating that this class is the parent processing object
public:
	void PublishInControl(CLAM::InControl*) {}	

private:
	void testInControlTmpl_DoControl_ChangesInternalState()
	{
		CLAM::InControlTmpl<ControlsTest> in("I'm an in ctrl template", this);// calls this->PublishInControl
		in.DoControl(1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, in.GetLastValue() );
	}
	// helper method used for handling incoming control
	int ControlHandler(CLAM::TControlData val) {
		_ost << "ControlHandler called with: " << val;
		return 0;
	}
	void testLinkAndSendWithInControlTmpl_CallbackMethodGetsCalled()
	{
		CLAM::InControlTmpl<ControlsTest> 
			in("in", this, &ControlsTest::ControlHandler); // calls this->PublishInControl
		
		in.DoControl(1.f);
		CPPUNIT_ASSERT_EQUAL( _ost.str(), std::string("ControlHandler called with: 1") );
		_ost.clear();
	}

	// helper method for handling incoming control plus incontrol ID
	int ControlHandlerId(int id, CLAM::TControlData val) {
		_ost << "ControlHandler called with id : " << id << " and value : " << val;
		return 0;
	}
	void testLinkAndSendWithInControlTmpl_CallbackWithIdMethodGetsCalled()
	{
		const int controlId=2;
		CLAM::InControlTmpl<ControlsTest> 
			in( controlId, "in", this, &ControlsTest::ControlHandlerId ); // calls this->PublishInControl
				
		in.DoControl( 1.f );
		CPPUNIT_ASSERT_EQUAL( 
			_ost.str(), 
			std::string("ControlHandler called with id : 2 and value : 1") );
		    // note that controlId == 2
		_ost.clear();
	}

	void testInControl_GetName_ChangesInteralState()
	{
		CLAM::InControl in("in name");
		CPPUNIT_ASSERT_EQUAL(std::string("in name"), in.GetName() );
	}
	void testOutControl_GetName_ChangesInteralState()
	{
		CLAM::OutControl out("out name");
		CPPUNIT_ASSERT_EQUAL(std::string("out name"), out.GetName() );
	}
};



	// dummy classes for testing
	class DummyConfig : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE (DummyConfig, 1, CLAM::ProcessingConfig)
		DYN_ATTRIBUTE (0, public, std::string, Name);
	};

	class DummyProcessing : public CLAM::Processing
	{
	public:
		DummyProcessing() { Configure( DummyConfig() ); }
		const char* GetClassName() const { return "DummyProcessing"; }
		bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return true; }
		const CLAM::ProcessingConfig &GetConfig() const { static DummyConfig _c; return _c; }
		bool Do() { return true; }
	};

	// dummy class for testing
	class DummyWithDynamicInControls : public DummyProcessing
	{
		std::vector<CLAM::InControl*> _inCtls;
		CLAM::OutControl _out;
	public:
		enum { size=6 };
		DummyWithDynamicInControls() : _out ("i'm an out", this)
		{
			for (int i=0; i<size; i++) {
				std::stringstream strm;
				strm << "i'm an in_" << i;
				_inCtls.push_back( new CLAM::InControl(strm.str(), this) ); 
				// new controls are published by default
			}
		}
	};

// todo: rename and place in processing test
void ControlsTest::ProcessingSidePublishedContainers()
{
	DummyWithDynamicInControls dum;
	const int last = DummyWithDynamicInControls::size - 1;
	// rise an exepected std::exception
	// some compiler uses insecure operator[] instead of .at
#ifdef HAVE_STANDARD_VECTOR_AT
		dum.GetInControl(last+1);
#else
		throw std::out_of_range("in his case, pass the test")
#endif
}

void ControlsTest::ProcessingSideInterface()
{
	DummyWithDynamicInControls sender, receiver;
	const int last = DummyWithDynamicInControls::size - 1;

	CPPUNIT_ASSERT_EQUAL( std::string("i'm an in_0"), receiver.GetInControl(0)->GetName() );
	CPPUNIT_ASSERT_EQUAL( std::string("i'm an in_1"), receiver.GetInControl(1)->GetName() );
	std::stringstream strm;
	strm << "i'm an in_" << last;
	CPPUNIT_ASSERT_EQUAL( strm.str(), receiver.GetInControl(last)->GetName() );

	sender.LinkOutWithInControl(0, &receiver, 0);
	sender.LinkOutWithInControl(0, &receiver, last);
	
	const TControlData _3(3);
	sender.SendControl( 0, _3 );
	CPPUNIT_ASSERT_EQUAL( _3, receiver.GetInControl(0)->GetLastValue() );
	CPPUNIT_ASSERT_EQUAL( _3, receiver.GetInControl(last)->GetLastValue() );
	const TControlData _0(0);
	CPPUNIT_ASSERT_EQUAL( _0, receiver.GetInControl(1)->GetLastValue() );
	// in control 1 wasn't connected. So it remains with the init value

}
} // namespace
