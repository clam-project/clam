#include "MiniCppUnit.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"

class TestsPortsRegionsInteraction : public GrupDeTests<TestsPortsRegionsInteraction>
{
public:
	GRUP_DE_TESTS(TestsPortsRegionsInteraction)
	{
		CAS_DE_TEST( testOutPort_connectToIn_usingBaseClass );
		CAS_DE_TEST( testWritingRegion_addRegion );
		CAS_DE_TEST( testWritingRegion_removeRegion );
		CAS_DE_TEST( testWritingRegion_removeRegion_withTwoReadingRegions );
		CAS_DE_TEST( testOutPort_connect_whenNoNode );
//		CAS_DE_TEST( testNode_content );
//		CAS_DE_TEST( testProduceAndConsume );
	}

	void testOutPort_connectToIn_usingBaseClass()
	{
		OutPort<int> out;
		InPort<int> in;
		OutPortBase& outBase = out;
		InPortBase& inBase = in;

		outBase.connectToIn(inBase);

		ASSERT_IGUALS( &inBase, outBase.connectedInPorts() );
	}
	void testWritingRegion_addRegion()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;

		writer.linkRegions( reader );
		Region & baseReader = (Region &)reader;
		ASSERT_IGUALS( &baseReader, *(writer.beginReaders()) );
	}
	void testWritingRegion_removeRegion()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;

		writer.linkRegions( reader );
		writer.removeRegion( reader );
		ASSERT( writer.beginReaders() == writer.endReaders() );
	}
	void testWritingRegion_removeRegion_withTwoReadingRegions()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader1, reader2;
		writer.linkRegions( reader1 );
		writer.linkRegions( reader2 );
		writer.removeRegion( reader1 );

		Region & baseReader2 = (Region &)reader2;
		ASSERT_IGUALS( &baseReader2, *writer.beginReaders() );
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
		ASSERT( existReadingRegion );

		WritingRegion<int> & writer = out.writingRegion();
		
		Region & baseReader = **(writer.beginReaders());
		WritingRegion<int>::ProperReadingRegion & reader = 
			(WritingRegion<int>::ProperReadingRegion &)baseReader;
		ASSERT( &(writer.stream()) == &(reader.stream()) );
	}

/*
	void testNode_content()
	{
		Node<int, DefaultStreamImpl> stream;
		stream.content(1);
		ASSERT_IGUALS( 1, stream.content() );
	}
*/

/*
	void testProduceAndConsume()
	{
		OutPort<int> out;
		InPort<int> in;

		out.connectToIn(in);
		out.produceData(1);
		ASSERT_IGUALS( 1, in.consumeData() );
	}
*/
	//TODO: next test: produceAndConsume
	// refactoring: regions knows stream and not out ports. then write region owns stream.
};




