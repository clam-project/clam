#include "MiniCppUnit.hxx"

#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"
#include <list>

/**
 * Into this test suite goes all specific tests that only aply to STL based
 * stream and not to phantom-buffer stream
 */
 //TODO it should be a template (stl container) class like TestsStream
class TestsStlBasedStream : public GrupDeTests<TestsStlBasedStream>
{
public:
	GRUP_DE_TESTS(TestsStlBasedStream)
	{
		CAS_DE_TEST( testStreamIncreasesSizeAfterProducing );
	}

	// tests of size of stream
	void testStreamIncreasesSizeAfterProducing()
	{
		WritingRegion<char,std::list> writer;
		writer.size(5);
		writer.hop(2);
		ASSERT_IGUALS(5, writer.logicalStreamSize() );
		writer.produce();
		ASSERT_IGUALS(7, writer.logicalStreamSize() );
	}

};
