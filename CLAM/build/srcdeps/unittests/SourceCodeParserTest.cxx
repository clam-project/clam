#include <cppunit/extensions/HelperMacros.h>

extern "C"
{
#	include "parser.h"
#	include "includepaths.h"
#	include "config_parser.h"
//#	include "dsp_parser.h"
}

namespace srcdepsTest
{


class SourceCodeParserTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SourceCodeParserTest );

class SourceCodeParserTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SourceCodeParserTest );

	CPPUNIT_TEST( test_parser_and_config_init_after_exit );
	CPPUNIT_TEST( test_guessed_sources_SizeInitially );
	CPPUNIT_TEST( test_parser_init_fills_includepath );
	CPPUNIT_TEST( test_search_include_list );
	CPPUNIT_TEST( test_HeaderFoundFromASource_InTheSameDir );
	CPPUNIT_TEST( test_inclusion_of_header_with_different_name );
	CPPUNIT_TEST( test_find_files_at_include_path );
	// the following test can not be automated since exit(-1) gets refactored to exceptions
//	CPPUNIT_TEST( test_error_impl_not_found_when_not_in_includepath );
	CPPUNIT_TEST( test_needed_includepaths_when_added_recursive_dir );
	CPPUNIT_TEST_SUITE_END();

	std::string mTestPath;
	std::string mCurrentFileName;

public:
	SourceCodeParserTest() : mTestPath("unittests/source_files_for_testing/")
	{
	}
	/// Common initialization, executed before each test method
	void setUp() 
	{ 
		config_init();
		parser_init();
		gendepend = 0;
		recursesrcs = 1;
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{
		parser_exit();
		config_exit();
	}

private:
	void test_parser_and_config_init_after_exit()
	{
		config_init();
		parser_init();
		//
		parser_exit();
		config_exit();
		//
		config_init();
		parser_init();
		CPPUNIT_ASSERT_EQUAL( 0, list_size(guessed_sources) );
		CPPUNIT_ASSERT_EQUAL( 0, list_size(library_paths) );
	}

	void test_search_include_list()
	{
		CPPUNIT_ASSERT_EQUAL( 0, list_size(includepaths) );
	}

	void test_guessed_sources_SizeInitially()
	{
		
		CPPUNIT_ASSERT_EQUAL( 0, list_size(guessed_sources) );

	}

	//! updates and returns the c_str of the mCurrentFileName. This member string contains all the path
	const char* helper_filename( const std::string nameWithoutPath )
	{
		mCurrentFileName = mTestPath + nameWithoutPath;
		return mCurrentFileName.c_str();

	}

/** 
  test files map:
  ---------------

  a.hxx   with_no_impl.hxx   b.hxx    subdir/c.hxx               e.hxx -> f.hxx -> g.hxx
    ^          ^             ^               ^                      ^                 ^
	|          |             |               |                      |                 |
  a.cxx         -----b.cxx---             --- --subdir/c.cxx     e.cxx             g.cxx
                                         |         
                                        d.cxx      
                                                   c.cxx

j.hxx       subdir/h.hxx  ->  subdir/subsubdir/i.hxx
  ^     -----^   ^                            ^
  |   /          |                            |
j.cxx - subdir/h.cxx      subdir/subsubdir/i.cxx


**/
	void test_HeaderFoundFromASource_InTheSameDir()
	{
		// PA: why call the whole parser_init() instead of initiallising just the needed structures?
		// (well, that's the way I starter) the parser needs a non-exported variable called includes_checked
		// so it's impossible to initialize it from outside.
		// the same aplyies to config_init()
		// TODO: refactori to object-oriented style, of course!

		parser_run( helper_filename("a.cxx") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("guessed_sources size", 1, list_size(guessed_sources) );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("guessed_headers size", 1, list_size(guessed_headers) );
		
		CPPUNIT_ASSERT_EQUAL( mTestPath+"a.hxx", std::string(guessed_headers->first->str) );
	}



	/** we want to test that b.cxx includes b.hxx and with_no_impl.hxx
	 * and we get only one (b.cxx) impl. file
	 */
	void test_inclusion_of_header_with_different_name()
	{
		parser_run( helper_filename("b.cxx") );

		CPPUNIT_ASSERT_EQUAL( 2, list_size(guessed_headers) );	
		CPPUNIT_ASSERT_EQUAL( 1, list_size(guessed_sources) );

		CPPUNIT_ASSERT_EQUAL(mTestPath+"b.hxx" , std::string( guessed_headers->first->str ) );	
		CPPUNIT_ASSERT_EQUAL(mTestPath+"with_no_impl.hxx" , std::string( guessed_headers->last->str ) );	
	}
	
	void test_parser_init_fills_includepath()
	{
		list* includes= listhash_find(config,"SEARCH_INCLUDES")->l;
		list_add_str_once(includes, helper_filename("subdir") );

		parser_init();
		CPPUNIT_ASSERT_EQUAL( 1, list_size(includepaths) );
		CPPUNIT_ASSERT_EQUAL( std::string(mTestPath+"subdir"), std::string(includepaths->first->str)  );
	}

	

	//! parsing d.cxx we get subdir/c.cxx and no c.cxx
	void test_find_files_at_include_path()
	{
		includepaths_add( helper_filename("subdir") );
		// tip: dir name ended with "/" doesn't work.
		 
		parser_run( helper_filename("d.cxx") );
		
		CPPUNIT_ASSERT_EQUAL_MESSAGE("should find only subdir/c.cxx", 1, list_size(guessed_sources) );
		CPPUNIT_ASSERT_EQUAL( mTestPath+"subdir/c.cxx", std::string( guessed_sources->first->str ) );


	}
		    
	void test_error_impl_not_found_when_not_in_includepath()
	{
		parser_run( helper_filename("d.cxx") );
		CPPUNIT_ASSERT_EQUAL(0, list_size(guessed_sources) );
		CPPUNIT_ASSERT_EQUAL(10, list_size(guessed_headers) );
	}

	/**
	 * parsing j.hxx and having subdir/ as recursive dir should result in the following:
	 * - needed_includes= subdir/ subdir/subsubdir/
	 * - guessed_sources = j.cxx, h.cxx, i.cxx
	 * - guessed_headers = j.hxx, h.hxx, i.hxx
	 */
	void test_needed_includepaths_when_added_recursive_dir()
	{
		includepaths_add_rec( helper_filename("subdir") );
		parser_run( helper_filename("j.cxx") );

		CPPUNIT_ASSERT_EQUAL_MESSAGE("querying needed_includepaths size", 2, list_size(needed_includepaths) );
		CPPUNIT_ASSERT_EQUAL( mTestPath+"subdir", std::string(needed_includepaths->last->str) );
		// TODO: "\" instead of "/" is a bug.
		CPPUNIT_ASSERT_EQUAL( mTestPath+"subdir\\subsubdir", std::string(needed_includepaths->first->str) );

		
		CPPUNIT_ASSERT_EQUAL_MESSAGE("querying guessed_sources size", 3, list_size(guessed_sources) );
		
		item* i = guessed_sources->first;
		CPPUNIT_ASSERT_EQUAL( mTestPath+"j.cxx", std::string(i->str) );
		i = i->next;
		CPPUNIT_ASSERT_EQUAL( mTestPath+"subdir\\subsubdir/i.cxx", std::string(i->str) );
		i = i->next;
		CPPUNIT_ASSERT_EQUAL( mTestPath+"subdir/h.cxx", std::string(i->str) );
		


		CPPUNIT_ASSERT_EQUAL_MESSAGE("querying guessed_headers size", 3, list_size(guessed_headers) );
		i = guessed_headers->first;
		CPPUNIT_ASSERT_EQUAL( mTestPath+"j.hxx", std::string(i->str) );
		i = i->next;
		CPPUNIT_ASSERT_EQUAL( mTestPath+"subdir\\subsubdir/i.hxx", std::string(i->str) );
		i = i->next;
		CPPUNIT_ASSERT_EQUAL( mTestPath+"subdir/h.hxx", std::string(i->str) );




	}

	// void test_priority_finding_headers()
	// begins looking at the .cxx file dir

	// void test_exits_with_error_if_finds_duplicated_header_name()
	// this test is not likely to pass with actual code (pau)

	// test(s) of #define #ifdef

	// test(s) of warnings emitted when facing complicated #ifdef logic

	// void test_recursive_includepath
	
	// void test_nonrecursive_inlcudepath

	// void efficiencytest_a_CLAM_real_case()
	// take a test that now have a duplicated-source-file bug
	




};









} // namespace srcdepsTest