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
	CPPUNIT_TEST_SUITE( ControlsTest );
	CPPUNIT_TEST( testInControl_DoControl_ChangesInternalState );
	CPPUNIT_TEST( testLinkAndSendControl_ChangesInControlInternalState );
	CPPUNIT_TEST( testInControlTmpl_DoControl_ChangesInternalState );
	CPPUNIT_TEST( testLinkAndSendWithInControlTmpl_CallbackMethodGetsCalled );
	CPPUNIT_TEST( testLinkAndSendWithInControlTmpl_CallbackWithIdMethodGetsCalled );
	CPPUNIT_TEST( testInControl_GetName_ChangesInteralState );
	CPPUNIT_TEST( testOutControl_GetName_ChangesInteralState );
	CPPUNIT_TEST_SUITE_END();
	
	// helper attribute
	std::stringstream _log;

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
		_log << "ControlHandler called with: " << val;
		return 0;
	}
	void testLinkAndSendWithInControlTmpl_CallbackMethodGetsCalled()
	{
		CLAM::InControlTmpl<ControlsTest> 
			in("in", this, &ControlsTest::ControlHandler); // calls this->PublishInControl
		
		in.DoControl(1.f);
		CPPUNIT_ASSERT_EQUAL( _log.str(), std::string("ControlHandler called with: 1") );
		_log.clear();
	}

	// helper method for handling incoming control plus incontrol ID
	int ControlHandlerId(int id, CLAM::TControlData val) {
		_log << "ControlHandler called with id : " << id << " and value : " << val;
		return 0;
	}
	void testLinkAndSendWithInControlTmpl_CallbackWithIdMethodGetsCalled()
	{
		const int controlId=2;
		CLAM::InControlTmpl<ControlsTest> 
			in( controlId, "in", this, &ControlsTest::ControlHandlerId ); // calls this->PublishInControl
				
		in.DoControl( 1.f );
		CPPUNIT_ASSERT_EQUAL( 
			_log.str(), 
			std::string("ControlHandler called with id : 2 and value : 1") );
		    // note that controlId == 2
		_log.clear();
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


} // namespace
