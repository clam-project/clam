#include <cppunit/extensions/HelperMacros.h>
#include "InPort.hxx"
#include "OutPort.hxx"

namespace CLAMTest {

class TestsPortsRegionsInteraction ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsPortsRegionsInteraction );

class TestsPortsRegionsInteraction : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsPortsRegionsInteraction );

	CPPUNIT_TEST( testOutPortConnectToIn_usingBaseClass );
	CPPUNIT_TEST( testOutPortConnect );
	CPPUNIT_TEST( testOutPortConnect_whenPortsAlreadyConnected ); 
	CPPUNIT_TEST( testOutPortConnect_whenInPortAlreadyConnectedToAnotherOutPort );
	CPPUNIT_TEST( testOutPortConnect_whenMoreThanOneInPort ); 
	CPPUNIT_TEST( testOutPortConnectNotifiesInPort );
	CPPUNIT_TEST( testOutPortDisconnect_whenPortsAreConnected );
	CPPUNIT_TEST( testOutPortDisconnect_whenPortsAreConnected_usingBaseClass );
	CPPUNIT_TEST( testOutPortDisconnect_whenPortsAreNotConnected_throwsException ); 
	CPPUNIT_TEST( testOutPortDisconnect_whenMoreThanOneInPort ); 
	CPPUNIT_TEST( testOutPortDisconnectNotifiesInPort );
	CPPUNIT_TEST( testPortChangeSizeResizesRegion ); 
	CPPUNIT_TEST( testProduceAndConsume );
	CPPUNIT_TEST( testProduceAndConsume_whenMoreThanOneInPort ); 
	CPPUNIT_TEST( testOutPortGetConnectedInPorts_whenOneInPort ); 
	CPPUNIT_TEST( testOutPortGetConnectedInPorts_whenMoreThanOneInPort );
	CPPUNIT_TEST( testInPortGetConnectedOutPort ); 
	CPPUNIT_TEST_SUITE_END();

	void testOutPortConnectToIn_usingBaseClass()
	{
		OutPort<int> out;
		InPort<int> in;
		OutPortBase& outBase = out;
		InPortBase& inBase = in;

		outBase.ConnectToIn(inBase);

		CPPUNIT_ASSERT_EQUAL( true, outBase.IsConnectedTo(inBase) );
	}
	
	void testOutPortConnect()
	{
		OutPort<int> out;
		InPort<int> in;
		out.ConnectToIn(in);
		//TODO check that stream has been initialized (as soon as is implemented)

		const bool existReadingRegion =
			out.GetRegion().BeginReaders() != out.GetRegion().EndReaders();
		CPPUNIT_ASSERT( existReadingRegion );

		WritingRegion<int> & writer = out.GetRegion();
		
		Region & baseReader = **(writer.BeginReaders());
		WritingRegion<int>::ProperReadingRegion & reader = 
			(WritingRegion<int>::ProperReadingRegion &)baseReader;
		CPPUNIT_ASSERT( &(writer.Stream()) == &(reader.Stream()) );
	}

	void testOutPortConnect_whenMoreThanOneInPort()
	{	
		OutPort<int> out;
		InPort<int> in1, in2;
		out.ConnectToIn(in1);
		out.ConnectToIn(in2);

		CPPUNIT_ASSERT_EQUAL( true, out.IsConnectedTo(in1) );
		CPPUNIT_ASSERT_EQUAL( true, out.IsConnectedTo(in2) );
	}

	void testOutPortConnect_whenPortsAlreadyConnected()
	{
		OutPort<int> out;
		InPort<int> in;
		out.ConnectToIn(in);

		try
		{
			out.ConnectToIn(in);	
			CPPUNIT_FAIL("exception should be thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
	}
		
	void testOutPortConnect_whenInPortAlreadyConnectedToAnotherOutPort()
	{
		OutPort<int> out, out2;
		InPort<int> in;
		out.ConnectToIn(in);

		try
		{
			out2.ConnectToIn(in);	
			CPPUNIT_FAIL("exception should be thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
	}

	void testOutPortConnectNotifiesInPort()
	{
		OutPort<int> out;
		InPort<int> in; 
		CPPUNIT_ASSERT( 0 == in.GetAttachedOutPort() );
		out.ConnectToConcreteIn(in);
		CPPUNIT_ASSERT( &out == in.GetAttachedOutPort() );
	}
	void testOutPortDisconnect_whenPortsAreConnected()
	{
		OutPort<int> out;
		InPort<int> in;		
		out.ConnectToConcreteIn(in);

		out.DisconnectFromConcreteIn( in );
		CPPUNIT_ASSERT(0 ==  in.GetRegion().ProducerRegion() );
		CPPUNIT_ASSERT( out.GetRegion().BeginReaders() == out.GetRegion().EndReaders() );
	}

	void testOutPortDisconnect_whenPortsAreConnected_usingBaseClass()
	{
		OutPort<int> out;
		InPort<int> in;
		OutPortBase& outBase = out;
		InPortBase& inBase = in;
		outBase.ConnectToIn(inBase);

		outBase.DisconnectFromIn(inBase);

		CPPUNIT_ASSERT_EQUAL( false, outBase.IsConnectedTo(inBase) );
	}

	void testOutPortDisconnect_whenPortsAreNotConnected_throwsException()
	{
		OutPort<int> out;
		InPort<int> in;		

		try
		{
			out.DisconnectFromConcreteIn( in );
			CPPUNIT_FAIL("exception should be thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
	}

	void testOutPortDisconnect_whenMoreThanOneInPort()
	{
		OutPort<int> out;
		InPort<int> in1, in2, in3;		
		out.ConnectToConcreteIn(in1);
		out.ConnectToConcreteIn(in2);
		out.ConnectToConcreteIn(in3);

		out.DisconnectFromConcreteIn(in2);

		CPPUNIT_ASSERT_EQUAL( false, out.IsConnectedTo(in2) );
	}

	void testOutPortDisconnectNotifiesInPort()
	{
		OutPort<int> out;
		InPort<int> in; 
		out.ConnectToConcreteIn(in);

		out.DisconnectFromConcreteIn(in);

		CPPUNIT_ASSERT( 0 == in.GetAttachedOutPort() );
	}

	void testPortChangeSizeResizesRegion()
	{
		OutPort<int> out;
		int newSize = 5;
		out.GetRegion().Size(5);

		CPPUNIT_ASSERT_EQUAL( newSize, out.GetRegion().Size() );
	}

	void testProduceAndConsume()
	{
		OutPort<int> out;
		InPort<int> in;
		int data = 4;

		out.ConnectToIn(in);
		out.GetRegion()[0] = 4;
		out.GetRegion().Produce();
		
		CPPUNIT_ASSERT_EQUAL( data, in.GetRegion()[0] );
	}

	void testProduceAndConsume_whenMoreThanOneInPort()
	{
		OutPort<int> out;
		InPort<int> in1, in2, in3;
		int data1 = 1;
		int data2 = 2;
		int data3 = 3;

		out.ConnectToConcreteIn(in1);
		out.ConnectToConcreteIn(in2);
		out.ConnectToConcreteIn(in3);

		in1.GetRegion().Size(2);
		in3.GetRegion().Size(3);

		out.GetRegion()[0] = data1;
		out.GetRegion().Produce();
		out.GetRegion()[0] = data2;
		out.GetRegion().Produce();
		out.GetRegion()[0] = data3;
		out.GetRegion().Produce();

		CPPUNIT_ASSERT_EQUAL( data1, in1.GetRegion()[0] );
		CPPUNIT_ASSERT_EQUAL( data2, in1.GetRegion()[1] );
		
		CPPUNIT_ASSERT_EQUAL( data1, in2.GetRegion()[0] );
		
		CPPUNIT_ASSERT_EQUAL( data1, in3.GetRegion()[0] );
		CPPUNIT_ASSERT_EQUAL( data2, in3.GetRegion()[1] );
		CPPUNIT_ASSERT_EQUAL( data3, in3.GetRegion()[2] );

	}
	void testOutPortGetConnectedInPorts_whenOneInPort()
	{
		OutPort<int> out;
		InPort<int> in;
		out.ConnectToIn( in );
		
		OutPortBase::InPortsList::iterator it = out.BeginConnectedInPorts(); 
		CPPUNIT_ASSERT( it != out.EndConnectedInPorts() );
		CPPUNIT_ASSERT( *(it++) = &in );
		CPPUNIT_ASSERT( it == out.EndConnectedInPorts() );
		
	}
	
	void testOutPortGetConnectedInPorts_whenMoreThanOneInPort()
	{	
		OutPort<int> out;
		InPort<int> in1, in2, in3;
		out.ConnectToIn( in1 );
		out.ConnectToIn( in2 );
		out.ConnectToIn( in3 );
		
		OutPortBase::InPortsList::iterator it = out.BeginConnectedInPorts(); 
		CPPUNIT_ASSERT( it != out.EndConnectedInPorts() );
		CPPUNIT_ASSERT( *(it++) = &in1 );
		CPPUNIT_ASSERT( *(it++) = &in2 );
		CPPUNIT_ASSERT( *(it++) = &in3 );
		CPPUNIT_ASSERT( it == out.EndConnectedInPorts() );

	}
	void testInPortGetConnectedOutPort()
	{
		OutPort<int> out;
		InPort<int> in1, in2;
		out.ConnectToIn( in1 );
		out.ConnectToIn( in2 );

		CPPUNIT_ASSERT( &out == in1.GetAttachedOutPort() );
		CPPUNIT_ASSERT( &out == in2.GetAttachedOutPort() );
	}
};

} // namespace CLAMTest 
