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

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // needed for assertion_traits<bool>
#include "Port.hxx"
#include "Processing.hxx"
#include "ProcessingData.hxx"

namespace CLAMTest {

class PortsTest;
CPPUNIT_TEST_SUITE_REGISTRATION( PortsTest );


class PortsTest : public CppUnit::TestFixture, public CLAM::Processing
{
	CPPUNIT_TEST_SUITE( PortsTest );
	// Tests for the Port base interface:
	CPPUNIT_TEST( testInPort_Attach_WithGeneralTemplateInPort_UpdatesConcretePortState );
	CPPUNIT_TEST( testInPort_Attach_WithGeneralTemplateInPort_BadTypeAssertionFails );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterConstruction );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterAttach );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterUnattach );
	//  todo: remove these Audio specific tests if the Audio template specialization is 
	//        no longer necessary (VC6 issue)
	CPPUNIT_TEST( testInPort_Attach_WithAudioInPort_UpdatesConcretePortState );
	CPPUNIT_TEST( testInPort_Attach_WithAudioInPort_BadTypeAssertionFails );
	CPPUNIT_TEST( testInPort_IsAttached_WithAudioInPort_AfterConstruction );
	CPPUNIT_TEST( testInPort_IsAttached_WithAudioInPort_AfterAttach );
	CPPUNIT_TEST( testInPort_IsAttached_WithAudioInPort_AfterUnattach );
	// end todo



	// Tests for the concrete PortTmpl classes
	CPPUNIT_TEST_SUITE_END();

	// Testing pattern: Self Shunt
	// Processing interface:
	const char* GetClassName() const { return "for testing"; }
	bool Do() { return false; }
	const CLAM::ProcessingConfig& GetConfig() const { throw 0; }
	bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return false; }
	

	class DummyProcessingData : public CLAM::ProcessingData
	{
		int _state;
	public:
		DYNAMIC_TYPE_USING_INTERFACE( DummyProcessingData, 0, CLAM::ProcessingData );
	public:
		void DefaultInit() { _state = -1; }

		void SetState(int val) { _state = val; }
		int GetState() { return _state; }
	};

	void testInPort_Attach_WithGeneralTemplateInPort_UpdatesConcretePortState()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<DummyProcessingData> concreteInPort("in-port", this, dummyLength);
		DummyProcessingData attachedData;
		attachedData.SetState(1);
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attachedData );	
		
		CPPUNIT_ASSERT_EQUAL( concreteInPort.GetData().GetState(), attachedData.GetState() );
	}

	void testInPort_Attach_WithAudioInPort_UpdatesConcretePortState()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<CLAM::Audio> concreteInPort("in-port", this, dummyLength);
		CLAM::Audio attachedAudio;
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attachedAudio );
		// compare by reference (pointer) because Audio haven't yet operator==
		CPPUNIT_ASSERT_EQUAL( &concreteInPort.GetData(), &attachedAudio );
	}

	void testInPort_Attach_WithGeneralTemplateInPort_BadTypeAssertionFails()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<DummyProcessingData> concreteInPort("in-port", this, dummyLength);
		CLAM::InPort &baseInPort = concreteInPort;
		CLAM::Audio attachedAudio;
		try {
			baseInPort.Attach( attachedAudio );	
			CPPUNIT_FAIL("assertion failed expected, but nothing happened");
		} catch(CLAM::ErrAssertionFailed& )	{}
	}

	void testInPort_Attach_WithAudioInPort_BadTypeAssertionFails()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<CLAM::Audio> concreteInPort("in-port", this, dummyLength);
		CLAM::InPort &baseInPort = concreteInPort;
		DummyProcessingData attachedDummy;
		try {
			baseInPort.Attach( attachedDummy );	
			CPPUNIT_FAIL("assertion failed expected, but nothing happened");
		} catch(CLAM::ErrAssertionFailed& )	{}
	}

	void testInPort_IsAttached_WithAudioInPort_AfterConstruction()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<CLAM::Audio> concreteInPort("in-port", this, dummyLength);
		CLAM::InPort &baseInPort = concreteInPort;
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}

	void testInPort_IsAttached_WithGeneralTemplateInPort_AfterConstruction()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<DummyProcessingData> concreteInPort("in-port", this, dummyLength);
		CLAM::InPort &baseInPort = concreteInPort;
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}

	void testInPort_IsAttached_WithGeneralTemplateInPort_AfterAttach()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<DummyProcessingData> concreteInPort("in-port", this, dummyLength);
		DummyProcessingData attached;
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attached );	

		CPPUNIT_ASSERT_EQUAL( true, concreteInPort.IsAttached() );
	}
	void testInPort_IsAttached_WithAudioInPort_AfterAttach()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<CLAM::Audio> concreteInPort("in-port", this, dummyLength);
		CLAM::Audio attached;
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attached );	

		CPPUNIT_ASSERT_EQUAL( true, baseInPort.IsAttached() );
	}

	void testInPort_IsAttached_WithAudioInPort_AfterUnattach()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<CLAM::Audio> concreteInPort("in-port", this, dummyLength);
		CLAM::Audio attached;
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attached );	
		baseInPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}
	void testInPort_IsAttached_WithGeneralTemplateInPort_AfterUnattach()
	{
		const int dummyLength = 0;
		CLAM::InPortTmpl<DummyProcessingData> concreteInPort("in-port", this, dummyLength);
		DummyProcessingData attached;
		CLAM::InPort &baseInPort = concreteInPort;
		
		baseInPort.Attach( attached );	
		baseInPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}
	//void testInPort_UnAttach_WithAudioInPort_WhenIsNotAttached()
	

};

} //namespace 
