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
	
	
	CPPUNIT_TEST( testAttachInPortToNode_WithGeneralTemplateInPort_GetsAttachedToConcreteNode );
	//CPPUNIT_TEST( testAttachPortsAndGetData_ReadsTheWrittenData );

/*	
	CPPUNIT_TEST( testInPort_Attach_WithGeneralTemplateInPort_BadTypeAssertionFails );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterConstruction );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterAttach );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterUnattach );
	CPPUNIT_TEST( testInPort_UnAttach_WithGeneralTemplateInPort_WhenIsNotAttached );
	
	//OutPorts
	CPPUNIT_TEST( testOutPort_Attach_WithGeneralTemplateOutPort_UpdatesConcretePortState );
	CPPUNIT_TEST( testOutPort_Attach_WithGeneralTemplateOutPort_BadTypeAssertionFails );
	CPPUNIT_TEST( testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterConstruction );
	CPPUNIT_TEST( testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterAttach );
	CPPUNIT_TEST( testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterUnattach );
	CPPUNIT_TEST( testOutPort_UnAttach_WithGeneralTemplateOutPort_WhenIsNotAttached );
*/	

	// Tests for the concrete PortTmpl classes
	CPPUNIT_TEST_SUITE_END();

	// Testing pattern: Self Shunt
	// Processing interface:
	const char* GetClassName() const { return "for testing"; }
	bool Do() { return false; }
	const CLAM::ProcessingConfig& GetConfig() const { throw 0; }
	bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return false; }
	

	void testAttachInPortToNode_WithGeneralTemplateInPort_GetsAttachedToConcreteNode()
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
		CPPUNIT_ASSERT_EQUAL( theNode, in.GetNode() );
	}
	// idem but with bad concrete node type (asserts)

	// idem but with Node<Audio> (two tests)
	

	/*
	void testAttachPortsAndGetData_ReadsTheWrittenData()
	{
		CLAM::InPortTmpl<DummyProcessingData> concreteInPort
		CLAM::InPort& in = concreteInPort;
		DummyProcessingData& returned = in.GetData();
		CPPUNIT_ASSERT_EQUAL( 1, returned.GetState() );
	}
	*/

};

} // namespace
