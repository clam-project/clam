#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"

#include "MiniCppUnit.hxx"

template <template <class> class DataStructure>
class TestsStream : public GrupDeTests< TestsStream<DataStructure> >
{
public:
	// GRUP_DE_TESTS( TestsStream<DataStructure> )
	TestsStream() : GrupDeTests< TestsStream<DataStructure> >( "TestsStream<DataStructure>" )
	{
		CAS_DE_TEST( testWritingRegion_constructor );
		CAS_DE_TEST( testReadingRegion_constructor );
		CAS_DE_TEST( testWritingRegion_canProduce_withNoReaders );
		CAS_DE_TEST( testWritingRegion_canProduce_withOneReader );
		CAS_DE_TEST( testWritingRegion_fulfilsInvariant_whenRegionsAreOverlapped );
		CAS_DE_TEST( testWritingRegion_fulfilsInvariant_whenReaderOverlapsAndSurpassesWriter );
		CAS_DE_TEST( testWritingRegion_fulfilsInvariant_whenReaderSurpassesWriterWithoutOverlapping );
		CAS_DE_TEST( testReadingRegion_canConsume_whenRegionsAreOverlapped );
		CAS_DE_TEST( testReadingRegion_canConsume_when2ReadingRegionsBehindAndNonOverlap );
		CAS_DE_TEST( testWritingRegion_produce_movesRegion );
		CAS_DE_TEST( testReadingRegion_consume_whenIsLegal_movesRegion );
		CAS_DE_TEST( testProduceConsumeData_withUnitaryRegions );
		CAS_DE_TEST( testProduceConsumeData_withSizedRegions );
		CAS_DE_TEST( testProduceConsumeData_withSizedRegions_writerProducesMultipleTimes );
		CAS_DE_TEST( testStreamImplementation_avancingALongWay_semiStressTest );
	}

	void testWritingRegion_constructor()
	{
		WritingRegion<int,DataStructure> writer;
		ASSERT(0 == writer.pos());
		ASSERT(1 == writer.size());
		ASSERT(1 == writer.hop());
	}

	void testReadingRegion_constructor()
	{
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		ASSERT(0 == reader.pos());
		ASSERT(1 == reader.size());
		ASSERT(1 == reader.hop());
	}

	void testWritingRegion_canProduce_withNoReaders()
	{
		WritingRegion<int,DataStructure> writer;
		ASSERT( true == writer.canProduce() );
	}

	void testWritingRegion_canProduce_withOneReader()
	{
		WritingRegion<int,DataStructure> writer;
		typename WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );
		ASSERT( true == writer.canProduce() );
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

		ASSERT( true == writer.fulfilsInvariant() );
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

		ASSERT( true == writer.fulfilsInvariant() );
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

		ASSERT( false == writer.fulfilsInvariant() );
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

		ASSERT( false == reader.canConsume() );
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

		ASSERT( true == reader2.canConsume() );
		ASSERT( true == reader1.canConsume() );
	}


	void testWritingRegion_produce_movesRegion()
	{
		WritingRegion<int,DataStructure> writer;
		writer.size(4);
		writer.hop(3);
		writer.produce();
		ASSERT_IGUALS( long(3), writer.pos() );
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

		ASSERT_IGUALS( long(2), reader.pos() );
		ASSERT( false == reader.canConsume() );
	}

	void testProduceConsumeData_withUnitaryRegions()
	{
		WritingRegion<char,DataStructure> writer;
		typename WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		writer[0] = 'a';
		writer.produce();
		ASSERT_IGUALS( 'a', reader[0] );
		reader.consume();
		ASSERT( false == reader.canConsume() );
	}

	void testProduceConsumeData_withSizedRegions()
	{
		WritingRegion<char,DataStructure> writer;
		typename WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		writer.size(5);
		writer.hop(5);
		ASSERT_IGUALS( 0, int(writer.pos()) );


		writer[0] = 'h';
		writer[1] = 'e';
		writer[2] = 'l';
		writer[3] = 'l';
		writer[4] = 'o';
		writer.produce();

		reader.size(3);
		reader.hop(2);
		ASSERT_IGUALS( 'h', reader[0] );
		ASSERT_IGUALS( 'e', reader[1] );
		ASSERT_IGUALS( 'l', reader[2] );

		reader.consume();
		ASSERT_IGUALS( 'l', reader[0] );
		ASSERT_IGUALS( 'l', reader[1] );
		ASSERT_IGUALS( 'o', reader[2] );

		reader.consume();
		ASSERT( false == reader.canConsume() );
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
		ASSERT( false == reader.canConsume() );

		writer[0] = ' ';
		writer[1] = 'w';
		writer[2] = 'o';
		writer[3] = 'r';
		writer[4] = 'l';
		writer[5] = 'd';
		writer.produce(); // 2

		reader.consume(); // 3
		reader.consume(); // 4

		ASSERT( false == reader.canConsume() );
		writer.produce();  // without producing here, the following indexed acceses
		ASSERT_IGUALS( 'r', reader[0] );  // to the reader would fail an assert
		ASSERT_IGUALS( 'l', reader[1] );
		ASSERT_IGUALS( 'd', reader[2] );
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

			ASSERT_IGUALS( actual, reader[0] );
			reader.consume();
		}
	}

	// tests buffer-size

	// tests stl impl deletes old elements

	// tests varying regions sizes

	// tests inplace regions



};
