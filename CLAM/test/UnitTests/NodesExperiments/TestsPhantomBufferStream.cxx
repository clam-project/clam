
#include <cppunit/extensions/HelperMacros.h>
#include "PhantomBuffer.hxx"
#include "WritingRegion.hxx"
#include <list>

// TODO to remove. Just for debugging purposes
#include <cctype>
void printbuffer(char* p, int size)
{
	std::cout << "\n\n\t0         1         2         3         4\n"
	"\t01234567890123456789012345678901234567890123456789\n\t";
	char notshowable = '·';
	for (int i=0; i<size; i++)
	{
		char foo = p[i];
		if (std::isalpha(foo) )
			std::cout << foo;
		else
			std::cout << notshowable;
	}
	std::cout << std::endl;
}
// end remove

namespace CLAMTest {

class TestsPhantomBufferStream ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsPhantomBufferStream );

class TestsPhantomBufferStream : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsPhantomBufferStream );
	
	// standard tests (the same for vector, list and phantom buffer
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

	// concrete tests for PhantomBuffer
	CPPUNIT_TEST( testStreamDontIncreasesSizeAfterProducing );
	CPPUNIT_TEST( testStreamIncreasesSizeAfterLinkingWithBiggerRegion );
	CPPUNIT_TEST( testPhantomBufferIncreasesAfterLinkingWithBiggerRegion );
	CPPUNIT_TEST( testInsertionPositionInLogicalZone_afterLinkingWithBiggerRegion );
	CPPUNIT_TEST( testLogicalSizeIncreases_whenWriterIncreasesItsSize );
	CPPUNIT_TEST( testLogicalSizeRemainsTheSame_whenWriterDecreasesItsSize );
	CPPUNIT_TEST( testLogicalSizeIncreases_whenReaderIncreasesItsSize );
	CPPUNIT_TEST( testWriterRegionCantProduce_whenOverlapsReadingRegion );
	CPPUNIT_TEST( testWriting_rearmostPos_when2ReadingRegions );
	CPPUNIT_TEST( testWriter_sizeReservesSizePowOf2 );
  	CPPUNIT_TEST( testPhantomZoneGetsUpdated_whenWroteInBeginningZone );
	CPPUNIT_TEST( testBeginningZoneGetsUpdated_whenWroteInPhantomZone );
	CPPUNIT_TEST( testPhantomZoneGetsUpdated_whenInsertedInLogicalZone );
	CPPUNIT_TEST( testIntegrityAfterBufferResize );
	CPPUNIT_TEST( testWriterResizes_whenRearmostReaderHasSameBeginDistance );



	CPPUNIT_TEST_SUITE_END();

	/*
	 *  Standard Testing
	 */

	void testWritingRegion_constructor()
	{
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		CPPUNIT_ASSERT(0 == writer.pos());
		CPPUNIT_ASSERT(1 == writer.size());
		CPPUNIT_ASSERT(1 == writer.hop());
	}

	void testReadingRegion_constructor()
	{
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader;
		CPPUNIT_ASSERT(0 == reader.pos());
		CPPUNIT_ASSERT(1 == reader.size());
		CPPUNIT_ASSERT(1 == reader.hop());
	}

	void testWritingRegion_canProduce_withNoReaders()
	{
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		CPPUNIT_ASSERT( true == writer.canProduce() );
	}

	void testWritingRegion_canProduce_withOneReader()
	{
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader;
		writer.linkRegions( reader );
		CPPUNIT_ASSERT( true == writer.canProduce() );
	}

	void testWritingRegion_fulfilsInvariant_whenRegionsAreOverlapped()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:  ^
		// W:  ^
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.linkRegions( reader );

		CPPUNIT_ASSERT( true == writer.fulfilsInvariant() );
	}

	void testWritingRegion_fulfilsInvariant_whenReaderOverlapsAndSurpassesWriter()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:   ^--^
		// W:  ^^
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader;
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
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader;
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
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader;
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
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader1;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader2;
		writer.linkRegions( reader1 );
		writer.linkRegions( reader2 );

		reader2.size(3);
		writer.pos(3);

		CPPUNIT_ASSERT( true == reader2.canConsume() );
		CPPUNIT_ASSERT( true == reader1.canConsume() );
	}


	void testWritingRegion_produce_movesRegion()
	{
		WritingRegion<int,CLAM::PhantomBuffer> writer;
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

		WritingRegion<int,CLAM::PhantomBuffer> writer;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader;
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
		WritingRegion<char,CLAM::PhantomBuffer> writer;
		WritingRegion<char,CLAM::PhantomBuffer>::ProperReadingRegion reader;
		writer.linkRegions( reader );

		writer[0] = 'a';
		writer.produce();
		CPPUNIT_ASSERT_EQUAL( 'a', reader[0] );
		reader.consume();
		CPPUNIT_ASSERT( false == reader.canConsume() );
	}

	void testProduceConsumeData_withSizedRegions()
	{
		WritingRegion<char,CLAM::PhantomBuffer> writer;
		WritingRegion<char,CLAM::PhantomBuffer>::ProperReadingRegion reader;
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
		WritingRegion<char,CLAM::PhantomBuffer> writer;
		WritingRegion<char,CLAM::PhantomBuffer>::ProperReadingRegion reader;
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
		WritingRegion<int,CLAM::PhantomBuffer> writer;
		WritingRegion<int,CLAM::PhantomBuffer>::ProperReadingRegion reader;
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


	/*
	 *
	 *   Concrete Phantom Buffer tests (checking phantom zone, overlapping, etc.
	 *
	 */
	void testStreamDontIncreasesSizeAfterProducing()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.size(5);
		writer.hop(2);
		int initialLogicalSize = writer.logicalStreamSize();
		writer.produce();
		CPPUNIT_ASSERT_EQUAL(initialLogicalSize, writer.logicalStreamSize() );
	}


	//tests of logicalSize changed when link with a bigger reading region
	void testStreamIncreasesSizeAfterLinkingWithBiggerRegion()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.size(5);
		writer.hop(2);
		int sizeWithALonelyWritingRegion = writer.logicalStreamSize();

		reader.size(9);
		reader.hop(2);
		writer.linkRegions(reader);

		CPPUNIT_ASSERT( sizeWithALonelyWritingRegion < writer.logicalStreamSize() );
	}

	// test phantom buffer increases after linking with bigger reading region
	void testPhantomBufferIncreasesAfterLinkingWithBiggerRegion()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.size(5);
		writer.hop(2);

		reader.size(9);
		reader.hop(2);
		int oldPhantomSize = writer.stream().phantomSize();
		writer.linkRegions(reader);

		CPPUNIT_ASSERT( oldPhantomSize < writer.stream().phantomSize() );
	}

	// test resize reading region which is not in the begining
	// here we want to test the insertion point.
	void testInsertionPositionInLogicalZone_afterLinkingWithBiggerRegion()
	{
	WritingRegion<char, CLAM::PhantomBuffer> writer;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.size(5);
		writer.hop(2);

		writer[0] = 'g';
		writer[1] = 'o';
		writer[2] = 'o';
		writer[3] = 'd';
		writer[4] = '\0';
		writer.produce();

		reader.size(9);
		reader.hop(2);

  		writer.linkRegions(reader);

		CPPUNIT_ASSERT_EQUAL( std::string("od"), 
				std::string( &(writer.stream().operator[](18))) ); 
		//we are inserting 16 elems + offset of 2
	}

	void testLogicalSizeIncreases_whenWriterIncreasesItsSize()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.size(5);
		writer.hop(2);
		int initialSize = writer.logicalStreamSize();
		writer.size(10);
		CPPUNIT_ASSERT(initialSize < writer.logicalStreamSize() );
	}

	void testLogicalSizeRemainsTheSame_whenWriterDecreasesItsSize()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.size(5);
		writer.hop(2);
		int initialSize = writer.logicalStreamSize();
		writer.size(3);
		CPPUNIT_ASSERT(initialSize == writer.logicalStreamSize() );
	}

	//tests of logicalSize changed when reader changes its size
	void testLogicalSizeIncreases_whenReaderIncreasesItsSize()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.size(5);
		writer.hop(2);

		reader.size(2);
		reader.hop(2);
		int initialSize = writer.logicalStreamSize();
		writer.linkRegions(reader);

		CPPUNIT_ASSERT( initialSize == writer.logicalStreamSize() );
		reader.size( 9 );

		CPPUNIT_ASSERT( initialSize < writer.logicalStreamSize() );
	}

	//tests of !canProduce (circular overlap)
	void testWriterRegionCantProduce_whenOverlapsReadingRegion()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.size(4);
		writer.hop(1); // so we can position it at the conflictive point
		reader.size(1);
		writer.linkRegions(reader);
		while( writer.pos() <= writer.logicalStreamSize() - writer.size() )
		{
			writer[0] = 'X';
			writer.produce();
		}
		CPPUNIT_ASSERT( writer.canProduce() == false );
	}

	void testWriting_rearmostPos_when2ReadingRegions()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion firstReader;
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion lastReader;
		writer.size(6);
		writer.hop(6);
		firstReader.size(4);
		firstReader.hop(4);
		lastReader.size(6);
		lastReader.hop(3);
		writer.linkRegions(firstReader);
		writer.linkRegions(lastReader);
		writer.produce();
		firstReader.consume();
		lastReader.consume();
		CPPUNIT_ASSERT_EQUAL( 3, writer.rearmostReadingPos() );
	}

	void testWriter_sizeReservesSizePowOf2()
	{
		int powOfNine = 512; // 512 == 2^9  (2^8 == 256)

		WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.size( 258 / 2 ); // logical size will be the power of 2 greater and closer to size*2
		CPPUNIT_ASSERT_EQUAL( powOfNine, writer.stream().logicalSize() );
	}

	void testPhantomZoneGetsUpdated_whenWroteInBeginningZone()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.size(5); // logical size == 16  (tbe pow of 2 > 5*2)
		writer.hop(5);
		//  food          |phan|
		//  0        1        2        3         4
		//  0·······90·······90·······90········90
		//1 ^---^
		//2      ^---^

		writer[0] = 'f';
		writer[1] = 'o';
		writer[2] = 'o';
		writer[3] = 'd';
		writer[4] = '\0';
		// it IS necessary to do a writer.produce() for updating phantom zone
		writer.produce();

		char *bufferbase = &(writer.stream().operator[](0) );
		//printbuffer(bufferbase, 22);

		// assert "food" == buffer[16 : 16+5]
		CPPUNIT_ASSERT_EQUAL( std::string("food"), std::string(bufferbase+16) );

	}

	// idem but accessing by the pointer
	// TODO

	void testBeginningZoneGetsUpdated_whenWroteInPhantomZone()
	{	

		WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.size(6); // logical size == 16  (tbe pow of 2 > 6*2)
		writer.hop(6);
		//             goodbye
		//  0        1        2        3         4
		//  0·······90·······90·······90········90
		//1 ^----^         [phn]
		//2       ^----^
		//3             ^----^
		char* bufferbase = &(writer.stream().operator[](0));
		
		writer.produce(); // now writer.pos == 6
		writer.produce(); // now writer.pos == 12
		char* toWrite = &writer[0];
		toWrite[0] = 'g';  // buffer[12]
		toWrite[1] = 'o';  // buffer[13]
		toWrite[2] = 'o';  // buffer[14]
		toWrite[3] = 'd';  // buffer[15]
		toWrite[4] = 'b';  // buffer[16] -- first position of phantom buffer
		toWrite[5] = 'y';
		writer.produce();
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;
		CPPUNIT_ASSERT_EQUAL('b', bufferbase[0] );
		CPPUNIT_ASSERT_EQUAL('y', bufferbase[1] );
	}

	void testPhantomZoneGetsUpdated_whenInsertedInLogicalZone()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.size(5); // logical size == 16  (tbe pow of 2 > 5*2)
		writer.hop(5);
		//  goodbye
		//  0        1        2        3         4
		//  0·······90·······90·······90········90
		//1 ^---^                        [phantm]
		//2      ^---^
		//3      ^-------^
		writer[0] = 'g';
		writer[1] = 'o';
		writer[2] = 'o';
		writer[3] = 'd';
		writer[4] = 'b';
		writer.produce();
		writer[0] = 'y'; // writer.pos == 5
		writer[1] = 'e'; // writer.pos == 6
		writer[2] = ' '; // writer.pos == 7
		writer[3] = 'h';
		writer[4] = 'i';
		//printbuffer( &(writer.stream().operator[](0)), 32 );
		// the following resize inserts at writer.pos == 5
		writer.size(9); // logical size == 32 (the pow of 2 > 9*2)

		//printbuffer( &(writer.stream().operator[](0)), 32+5 );

		char* bufferbase = &(writer.stream().operator[](0));
		// assert stream[16+5:16+5+2] == "ye"
		CPPUNIT_ASSERT_EQUAL('y', bufferbase[21] );
		CPPUNIT_ASSERT_EQUAL('e', bufferbase[22] );

		// assert stream[32:32+9] == stream[0:9] == "goodbXXye" where X are uninitialized chars
		CPPUNIT_ASSERT_EQUAL('g', bufferbase[32] );
		CPPUNIT_ASSERT_EQUAL('o', bufferbase[33] );
		CPPUNIT_ASSERT_EQUAL('o', bufferbase[34] );
		CPPUNIT_ASSERT_EQUAL('d', bufferbase[35] );
		CPPUNIT_ASSERT_EQUAL('b', bufferbase[36] );
	}
	void testIntegrityAfterBufferResize()
	{
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.size(1); // buff size == 2
		writer.hop(1);
		writer[0]='A';
		writer.produce();
		writer[0]='B';
		writer.produce();
		writer.size(2); // buff size == 4
		writer.hop(2);
		writer[0]='X';
		writer[1]='X';
		writer.produce();
		CPPUNIT_ASSERT_EQUAL('A', writer[0]);
	}
	
	void testWriterResizes_whenRearmostReaderHasSameBeginDistance()
	{	
		WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.size(3); // buff size = 8
		writer.hop(3);
	
		WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;
		writer.linkRegions(reader);
		reader.size(1);
		reader.hop(1);

		writer.produce(); // pos = 3, beginDistance = 3
		writer.produce(); // pos = 6, beginDistance = 6
		reader.consume(); // pos = 1, beginDistance = 1
		writer.produce(); // pos = 9, beginDistance = 1
		int oldBeginDistance = reader.beginDistance();
		
		writer.size(5);   // buff size = 16
		CPPUNIT_ASSERT( oldBeginDistance != reader.beginDistance() );
	}
};






} // namespace CLAMTest 
