#include <cppunit/extensions/HelperMacros.h>

extern "C"
{
#	include "parser.h"
#	include "includepaths.h"
//#	include "config_parser.h"
//#	include "dsp_parser.h"
}

namespace srcdepsTest
{


class SourceCodeParserTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SourceCodeParserTest );

class SourceCodeParserTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SourceCodeParserTest );

	CPPUNIT_TEST( test_guessed_sources_SizeInitially );
	CPPUNIT_TEST( test_HeaderFoundFromASource_InTheSameDir );
	CPPUNIT_TEST_SUITE_END();

	char* src_test_path;

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{ 
		src_test_path = "./source_files_for_testing/";
	}

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void test_guessed_sources_SizeInitially()
	{
		guessed_sources = list_new();
		
		CPPUNIT_ASSERT_EQUAL( 0, list_size(guessed_sources) );

		list_free( guessed_sources );
	}


	//	item* i = guessed_sources->first;
	//	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string( i->str ) );
	
	void test_HeaderFoundFromASource_InTheSameDir()
	{
		guessed_sources = list_new();
		guessed_headers = list_new();
		parser_run("a.cxx");
		CPPUNIT_ASSERT_EQUAL_MESSAGE("guessed_sources size", 0, list_size(guessed_sources) );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("guessed_headers size", 1, list_size(guessed_headers) );
		
		item* i = guessed_headers->first;
		CPPUNIT_ASSERT_EQUAL( std::string("a.h"), std::string(i->str) );
	}

};









} // namespace srcdepsTest