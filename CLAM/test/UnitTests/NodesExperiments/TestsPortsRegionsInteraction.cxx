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

		outBase.connectToIn(inBase);

		CPPUNIT_ASSERT_EQUAL( &inBase, outBase.connectedInPorts() );
	}
	void testWritingRegion_addRegion()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;

		writer.linkRegions( reader );
		Region & baseReader = (Region &)reader;
		CPPUNIT_ASSERT_EQUAL( &baseReader, *(writer.beginReaders()) );
	}
	void testWritingRegion_removeRegion()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;

		writer.linkRegions( reader );
		writer.removeRegion( reader );
		CPPUNIT_ASSERT( writer.beginReaders() == writer.endReaders() );
	}
	void testWritingRegion_removeRegion_withTwoReadingRegions()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader1, reader2;
		writer.linkRegions( reader1 );
		writer.linkRegions( reader2 );
		writer.removeRegion( reader1 );

		Region & baseReader2 = (Region &)reader2;
		CPPUNIT_ASSERT_EQUAL( &baseReader2, *writer.beginReaders() );
	}

	class OutPortStub : public OutPort<int>
	{
	public:
		WritingRegion<int>& writingRegion()
		{
			return _region;
		}
	};

	void testOutPort_connect_whenNoNode()
	{
		OutPortStub out;
		InPort<int> in;
		out.connectToIn(in);
		//TODO check that stream has been initialized (as soon as is implemented)

		const bool existReadingRegion =
			out.writingRegion().beginReaders() != out.writingRegion().endReaders();
		CPPUNIT_ASSERT( existReadingRegion );

		WritingRegion<int> & writer = out.writingRegion();
		
		Region & baseReader = **(writer.beginReaders());
		WritingRegion<int>::ProperReadingRegion & reader = 
			(WritingRegion<int>::ProperReadingRegion &)baseReader;
		CPPUNIT_ASSERT( &(writer.stream()) == &(reader.stream()) );
	}

/*
	void testNode_content()
	{
		Node<int, DefaultStreamImpl> stream;
		stream.content(1);
		CPPUNIT_ASSERT_EQUAL( 1, stream.content() );
	}
*/

/*
	void testProduceAndConsume()
	{
		OutPort<int> out;
		InPort<int> in;

		out.connectToIn(in);
		out.produceData(1);
		CPPUNIT_ASSERT_EQUAL( 1, in.consumeData() );
	}
*/
	//TODO: next test: produceAndConsume
	// refactoring: regions knows stream and not out ports. then write region owns stream.
};

} // namespace CLAMTest 
