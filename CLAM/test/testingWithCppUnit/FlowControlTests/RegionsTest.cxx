#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "StreamBuffer.hxx"
#include "ReadStreamRegion.hxx"
#include "WriteStreamRegion.hxx"
#include "Audio.hxx"
#include "CircularStreamImpl.hxx"

namespace CLAMTest
{


class RegionsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( RegionsTest );

class RegionsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( RegionsTest );
	CPPUNIT_TEST( testReadStreamRegion_CanActivate_WhenWriterIsJustInitialized );
	CPPUNIT_TEST( testWriteStreamRegion_CanActivate_WhenWriterIsJustInitialized );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void testReadStreamRegion_CanActivate_WhenWriterIsJustInitialized()
	{
		// Setup
		const int hop = 0;
		const int length = 1;
		CLAM::StreamBuffer<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > buffer;
		// Exercise
		CLAM::WriteStreamRegion * write = buffer.NewWriter( hop, length );
		CLAM::ReadStreamRegion * read = buffer.NewReader( hop, length );

		// Verification
		CPPUNIT_ASSERT_EQUAL( false, read->CanActivate() );
		
		// Tear down
		delete write;
		delete read;
	}
	
	void testWriteStreamRegion_CanActivate_WhenWriterIsJustInitialized()
	{
		// Setup
		const int hop = 0;
		const int length = 1;
		CLAM::StreamBuffer<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > buffer;
		// Exercise
		CLAM::WriteStreamRegion * write = buffer.NewWriter( hop, length );
		CLAM::ReadStreamRegion * read = buffer.NewReader( hop, length );

		// Verification
		CPPUNIT_ASSERT_EQUAL( true, write->CanActivate() );
		
		// Tear down
		delete write;
		delete read;
	}
};






} // namespace CLAMTest
