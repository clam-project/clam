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

	CPPUNIT_TEST( testOutPort_connectToIn_usingBaseClass );
	CPPUNIT_TEST( testWritingRegion_addRegion );
	CPPUNIT_TEST( testWritingRegion_removeRegion );
	CPPUNIT_TEST( testWritingRegion_removeRegion_withTwoReadingRegions );
	CPPUNIT_TEST( testOutPort_connect_whenNoNode );
//	CPPUNIT_TEST( testNode_content );
//	CPPUNIT_TEST( testProduceAndConsume );

	CPPUNIT_TEST_SUITE_END();

	void testOutPort_connectToIn_usingBaseClass()
	{
		OutPort<int> out;
		InPort<int> in;
		OutPortBase& outBase = out;
		InPortBase& inBase = in;

		outBase.ConnectToIn(inBase);

		CPPUNIT_ASSERT_EQUAL( &inBase, outBase.ConnectedInPorts() );
	}
	void testWritingRegion_addRegion()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;

		writer.LinkRegions( reader );
		Region & baseReader = (Region &)reader;
		CPPUNIT_ASSERT_EQUAL( &baseReader, *(writer.BeginReaders()) );
	}
	void testWritingRegion_removeRegion()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;

		writer.LinkRegions( reader );
		writer.RemoveRegion( reader );
		CPPUNIT_ASSERT( writer.BeginReaders() == writer.EndReaders() );
	}
	void testWritingRegion_removeRegion_withTwoReadingRegions()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader1, reader2;
		writer.LinkRegions( reader1 );
		writer.LinkRegions( reader2 );
		writer.RemoveRegion( reader1 );

		Region & baseReader2 = (Region &)reader2;
		CPPUNIT_ASSERT_EQUAL( &baseReader2, *writer.BeginReaders() );
	}

	class OutPortStub : public OutPort<int>
	{
	public:
		WritingRegion<int>& WritingRegion()
		{
			return mRegion;
		}
	};

	void testOutPort_connect_whenNoNode()
	{
		OutPortStub out;
		InPort<int> in;
		out.ConnectToIn(in);
		//TODO check that stream has been initialized (as soon as is implemented)

		const bool existReadingRegion =
			out.WritingRegion().BeginReaders() != out.WritingRegion().EndReaders();
		CPPUNIT_ASSERT( existReadingRegion );

		WritingRegion<int> & writer = out.WritingRegion();
		
		Region & baseReader = **(writer.BeginReaders());
		WritingRegion<int>::ProperReadingRegion & reader = 
			(WritingRegion<int>::ProperReadingRegion &)baseReader;
		CPPUNIT_ASSERT( &(writer.Stream()) == &(reader.Stream()) );
	}

/*
	void testNode_content()
	{
		Node<int, DefaultStreamImpl> stream;
		stream.Content(1);
		CPPUNIT_ASSERT_EQUAL( 1, stream.Content() );
	}
*/

/*
	void TestProduceAndConsume()
	{
		OutPort<int> out;
		InPort<int> in;

		out.ConnectToIn(in);
		out.ProduceData(1);
		CPPUNIT_ASSERT_EQUAL( 1, in.ConsumeData() );
	}
*/
	//TODO: next test: produceAndConsume
	// refactoring: regions knows stream and not out ports. then write region owns stream.
};

} // namespace CLAMTest 
