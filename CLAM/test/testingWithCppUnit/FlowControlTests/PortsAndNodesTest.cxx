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
//#include "cppUnitHelper.hxx" // needed for assertion_traits<bool>
#include "InPort.hxx"
#include "InPortTmpl.hxx"
#include "OutPort.hxx"
#include "OutPortTmpl.hxx"
#include "InPort.hxx"
#include "Processing.hxx"
#include "DummyProcessingData.hxx"
#include "NodeTmpl.hxx"
#include "CircularStreamImpl.hxx"


namespace CLAMTest {

class PortsAndNodesTest;
CPPUNIT_TEST_SUITE_REGISTRATION( PortsAndNodesTest );


class PortsAndNodesTest : public CppUnit::TestFixture, public CLAM::Processing
{
	CPPUNIT_TEST_SUITE( PortsAndNodesTest );
	// Tests for the Port base interface and Node:
	
	
	CPPUNIT_TEST( testAttachPortsToNode_WithGeneralTemplatePorts_GetsAttachedToConcreteNode );
	CPPUNIT_TEST( testAttachOutPortToNode_WithWrongNodeTypeAsserts );
	CPPUNIT_TEST( testAttachPortsToNode_WithAudioPorts_GetsAttachedToConcreteNode );
	CPPUNIT_TEST( testAttachPortsAndGetData_WithGeneralTemplatePorts_ReadsTheWrittenData );
	CPPUNIT_TEST( testAttachPortsAndGetData_WithAudioPorts_ReadsTheWrittenData );

	// Tests for the concrete PortTmpl classes
	CPPUNIT_TEST_SUITE_END();

	// Testing pattern: Self Shunt
	// Processing interface:
	const char* GetClassName() const { return "for testing"; }
	bool Do() { return false; }
	const CLAM::ProcessingConfig& GetConfig() const { throw 0; }
	bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return false; }
	

	void testAttachPortsToNode_WithGeneralTemplatePorts_GetsAttachedToConcreteNode()
	{
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;
		CLAM::InPortTmpl<DummyProcessingData> concreteInPort("in", this, 0/*dummy length*/);
		CLAM::InPort& in = concreteInPort;

		CLAM::OutPortTmpl<DummyProcessingData> concreteOutPort("out", this, 0/*dummy length*/);
		CLAM::OutPort& out = concreteOutPort;

		out.Attach(concreteNode);
		in.Attach(concreteNode);

		CLAM::NodeBase* theNode = &concreteNode;
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Node not attached", theNode, in.GetNode() );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Node not attached", theNode, out.GetNode() );
	}
	
	void testAttachOutPortToNode_WithWrongNodeTypeAsserts()
	{
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;
		CLAM::OutPortTmpl<CLAM::Audio> audioOutPort("out", this, 0/*dummy length*/);
		CLAM::OutPort& out = audioOutPort;
		
		try {
			out.Attach(concreteNode);
			CPPUNIT_FAIL("Assert expected but nothing happened");
		} catch ( CLAM::ErrAssertionFailed& ) {}
	}

	void testAttachPortsToNode_WithAudioPorts_GetsAttachedToConcreteNode()
	{
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;

		const int dummyLength=0; 
		CLAM::InPortTmpl<CLAM::Audio> concreteInPort("in", this, dummyLength);
		CLAM::InPort& in = concreteInPort;

		CLAM::OutPortTmpl<CLAM::Audio> concreteOutPort("out", this, dummyLength);
		CLAM::OutPort& out = concreteOutPort;

        out.Attach(concreteNode);
		in.Attach(concreteNode);

		CLAM::NodeBase* theNode = &concreteNode;
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Node not attached", theNode, in.GetNode() );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Node not attached", theNode, out.GetNode() );
	}
	
	void testAttachPortsAndGetData_WithGeneralTemplatePorts_ReadsTheWrittenData()
	{
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;

		const int length=1; 
		CLAM::InPortTmpl<DummyProcessingData> concreteIn("in", this, length);
		CLAM::InPort& in = concreteIn;
	
		CLAM::OutPortTmpl<DummyProcessingData> concreteOut("out", this, length);
		CLAM::OutPort& out = concreteOut;
		
		// attach using generic interface
		out.Attach(concreteNode);
		in.Attach(concreteNode);
		
		// configure node before using its data
		concreteNode.Configure(length);

		// write with out port
		concreteOut.GetData().SetState(1);
		concreteOut.LeaveData();

		DummyProcessingData& returned = concreteIn.GetData();
		CPPUNIT_ASSERT_EQUAL( 1, returned.GetState() );
	}

	void testAttachPortsAndGetData_WithAudioPorts_ReadsTheWrittenData()
	{
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;

		const int samples=1; 
		CLAM::InPortTmpl<CLAM::Audio> concreteIn("in", this, samples);
		CLAM::InPort& in = concreteIn;
	
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut("out", this, samples);
		CLAM::OutPort& out = concreteOut;
		
		// attach using generic interface
		out.Attach(concreteNode);
		in.Attach(concreteNode);
		
		// configure node before using its data
		concreteNode.Configure(samples);

		// write with out port

		concreteOut.GetData().GetBuffer()[0]=1;
		concreteOut.LeaveData();

		CLAM::Audio returned = concreteIn.GetData();
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(1), returned.GetBuffer()[0] );
		CPPUNIT_ASSERT_EQUAL( 1, returned.GetSize() );
	}
	

};

} // namespace
