#include <cppunit/extensions/HelperMacros.h>

#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"
#include <list>

/**
 * Into this test suite goes all specific tests that only aply to STL based
 * stream and not to phantom-buffer stream
 */
 //TODO it should be a template (stl container) class like TestsStream
namespace CLAMTest {

class TestsStlBasedStream ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsStlBasedStream );

class TestsStlBasedStream : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsStlBasedStream );

	CPPUNIT_TEST( testStreamIncreasesSizeAfterProducing );

	CPPUNIT_TEST_SUITE_END();

	// tests of size of stream
	void testStreamIncreasesSizeAfterProducing()
	{
		WritingRegion<char,std::list> writer;
		writer.size(5);
		writer.hop(2);
		CPPUNIT_ASSERT_EQUAL(5, writer.logicalStreamSize() );
		writer.produce();
		CPPUNIT_ASSERT_EQUAL(7, writer.logicalStreamSize() );
	}

};

} // namespace CLAMTest 
