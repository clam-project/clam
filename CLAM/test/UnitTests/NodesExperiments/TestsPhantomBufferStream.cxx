
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
