#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"

#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include <list>
#include "PhantomBuffer.hxx"

namespace CLAMTest {

// Test suite registration at the end of file
// That's due to the fact that we register template specialization of the suite tmpl class
//
template <template <class> class DataStructure>
class TestsStream : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsStream );
	
	CPPUNIT_TEST( testWritingRegion_constructor );
	CPPUNIT_TEST( testReadingRegion_constructor );
	CPPUNIT_TEST( testWritingRegion_canProduce_withNoReaders );
	CPPUNIT_TEST( testWritingRegion_canProduce_withOneReader );
	CPPUNIT_TEST( testWritingRegion_fulfilsInvariant_whenRegionsAreOverlapped );
	CPPUNIT_TEST( testWritingRegion_fulfilsInvariant_whenReaderOverlapsAndSurpassesWriter );
	CPPUNIT_TEST( testWritingRegion_fulfilsInvariant_whenReaderSurpassesWriterWithoutOverlapping );
	CPPUNIT_TEST( testReadingRegion_canConsume_whenRegionsAreOverlapped );
	CPPUNIT_TEST( testReadingRegion_canConsume_when2ReadingRegionsBehindAndNonOverlap );
	CPPUNIT_TEST( testWritingRegion_produce_movesRegion );
	CPPUNIT_TEST( testReadingRegion_consume_whenIsLegal_movesRegion );
	CPPUNIT_TEST( testProduceConsumeData_withUnitaryRegions );
	CPPUNIT_TEST( testProduceConsumeData_withSizedRegions );
	CPPUNIT_TEST( testProduceConsumeData_withSizedRegions_writerProducesMultipleTimes );
	CPPUNIT_TEST( testStreamImplementation_avancingALongWay_semiStressTest );

	CPPUNIT_TEST_SUITE_END();

	void testWritingRegion_constructor()
	{
		WritingRegion<int,DataStructure> writer;
		CPPUNIT_ASSERT(0 == writer.pos());
		CPPUNIT_ASSERT(1 == writer.size());
		CPPUNIT_ASSERT(1 == writer.hop());
	}

	void testReadingRegion_constructor()
	{
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		CPPUNIT_ASSERT(0 == reader.pos());
		CPPUNIT_ASSERT(1 == reader.size());
		CPPUNIT_ASSERT(1 == reader.hop());
	}

	void testWritingRegion_canProduce_withNoReaders()
	{
		WritingRegion<int,DataStructure> writer;
		CPPUNIT_ASSERT( true == writer.canProduce() );
	}

	void testWritingRegion_canProduce_withOneReader()
	{
		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );
		CPPUNIT_ASSERT( true == writer.canProduce() );
	}

	void testWritingRegion_fulfilsInvariant_whenRegionsAreOverlapped()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:  ^
		// W:  ^
		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;

		writer.linkRegions( reader );

		CPPUNIT_ASSERT( true == writer.fulfilsInvariant() );
	}

	void testWritingRegion_fulfilsInvariant_whenReaderOverlapsAndSurpassesWriter()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:   ^--^
		// W:  ^^
		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		writer.size(2);
		reader.pos(1);
		reader.size(4);

		CPPUNIT_ASSERT( true == writer.fulfilsInvariant() );
	}

	void testWritingRegion_fulfilsInvariant_whenReaderSurpassesWriterWithoutOverlapping()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:      ^--^
		// W:  ^--^
		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		writer.size(4);
		reader.size(4);
		reader.pos(4);

		CPPUNIT_ASSERT( false == writer.fulfilsInvariant() );
	}


	void testReadingRegion_canConsume_whenRegionsAreOverlapped()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:  ^
		// W:  ^
		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		CPPUNIT_ASSERT( false == reader.canConsume() );
	}

	void testReadingRegion_canConsume_when2ReadingRegionsBehindAndNonOverlap()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R1: ^
		// R2: ^-^
		// W:     ^
		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader1;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader2;
		writer.linkRegions( reader1 );
		writer.linkRegions( reader2 );

		reader2.size(3);
		writer.pos(3);

		CPPUNIT_ASSERT( true == reader2.canConsume() );
		CPPUNIT_ASSERT( true == reader1.canConsume() );
	}


	void testWritingRegion_produce_movesRegion()
	{
		WritingRegion<int,DataStructure> writer;
		writer.size(4);
		writer.hop(3);
		writer.produce();
		CPPUNIT_ASSERT_EQUAL( long(3), writer.pos() );
	}

	void testReadingRegion_consume_whenIsLegal_movesRegion()
	{
		//         |0         1         2
		//         |012345678901234567890
		// R_init:  ^--^
		// R_final:   ^--^
		// W:           ^-^
		//

		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		writer.size(3);
		writer.pos(4);
		reader.size(4);
		reader.hop(2);

		reader.consume();

		CPPUNIT_ASSERT_EQUAL( long(2), reader.pos() );
		CPPUNIT_ASSERT( false == reader.canConsume() );
	}

	void testProduceConsumeData_withUnitaryRegions()
	{
		WritingRegion<char,DataStructure> writer;
		typename WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		writer[0] = 'a';
		writer.produce();
		CPPUNIT_ASSERT_EQUAL( 'a', reader[0] );
		reader.consume();
		CPPUNIT_ASSERT( false == reader.canConsume() );
	}

	void testProduceConsumeData_withSizedRegions()
	{
		WritingRegion<char,DataStructure> writer;
		typename WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		writer.size(5);
		writer.hop(5);
		CPPUNIT_ASSERT_EQUAL( 0, int(writer.pos()) );


		writer[0] = 'h';
		writer[1] = 'e';
		writer[2] = 'l';
		writer[3] = 'l';
		writer[4] = 'o';
		writer.produce();

		reader.size(3);
		reader.hop(2);
		CPPUNIT_ASSERT_EQUAL( 'h', reader[0] );
		CPPUNIT_ASSERT_EQUAL( 'e', reader[1] );
		CPPUNIT_ASSERT_EQUAL( 'l', reader[2] );

		reader.consume();
		CPPUNIT_ASSERT_EQUAL( 'l', reader[0] );
		CPPUNIT_ASSERT_EQUAL( 'l', reader[1] );
		CPPUNIT_ASSERT_EQUAL( 'o', reader[2] );

		reader.consume();
		CPPUNIT_ASSERT( false == reader.canConsume() );
	}

	void testProduceConsumeData_withSizedRegions_writerProducesMultipleTimes()
	{
		WritingRegion<char,DataStructure> writer;
		typename WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		//       	|hello world
		// W         ^--------^  (hop=5)
		// R		 ^-^         (hop=2)
		//#consume
		//   #produce
		//    1		      ^-------^
		// 1		   ^-^
		// 2		     ^-^
		//    2                ^--------^
		// 3		       ^-^
		//    3		                ^--------^
		// 4		         ^-^

		writer.size(10);
		writer.hop(5);
		writer[0] = 'h';
		writer[1] = 'e';
		writer[2] = 'l';
		writer[3] = 'l';
		writer[4] = 'o';
		writer.produce();  // 1

		reader.size(3);
		reader.hop(2);

		reader.consume();  // 1
		reader.consume();  // 2
		CPPUNIT_ASSERT( false == reader.canConsume() );

		writer[0] = ' ';
		writer[1] = 'w';
		writer[2] = 'o';
		writer[3] = 'r';
		writer[4] = 'l';
		writer[5] = 'd';
		writer.produce(); // 2

		reader.consume(); // 3
		reader.consume(); // 4

		CPPUNIT_ASSERT( false == reader.canConsume() );
		writer.produce();  // without producing here, the following indexed acceses
		CPPUNIT_ASSERT_EQUAL( 'r', reader[0] );  // to the reader would fail an assert
		CPPUNIT_ASSERT_EQUAL( 'l', reader[1] );
		CPPUNIT_ASSERT_EQUAL( 'd', reader[2] );
	}

	void testStreamImplementation_avancingALongWay_semiStressTest()
	{
		const int lastTokenToConsume = 500;
		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );
		for (int actual=0; actual < lastTokenToConsume; actual++)
		{
			writer[0] = actual;
			writer.produce();

			CPPUNIT_ASSERT_EQUAL( actual, reader[0] );
			reader.consume();
		}
	}

	// tests buffer-size

	// tests stl impl deletes old elements

	// tests varying regions sizes

	// tests inplace regions



};

CPPUNIT_TEST_SUITE_REGISTRATION( TestsStream<CLAM::PhantomBuffer> );
CPPUNIT_TEST_SUITE_REGISTRATION( TestsStream<std::vector> );
CPPUNIT_TEST_SUITE_REGISTRATION( TestsStream<std::list> );

} // namespace CLAMTest 
