#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "Array.hxx"

namespace CLAMTest
{


class ArrayTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ArrayTest );

class ArrayTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ArrayTest );

	CPPUNIT_TEST( testArray_Constructing_NonMemoryOwning_With_Null_Pointer );
	CPPUNIT_TEST( testArray_Constructing_NonMemoryOwning_With_Valid_Pointer );

	CPPUNIT_TEST( testCopy_MemoryOwning_Into_MemoryOwning );
	CPPUNIT_TEST( testCopy_Being_The_Array_The_Placeholder_For_A_WriteStreamRegion );

	CPPUNIT_TEST( test_SetPtr_OnEmptyArray );
	CPPUNIT_TEST( test_TwoConsecutiveSetPtrs_OnEmptyArray );
	CPPUNIT_TEST( test_SetPtr_OnNonEmptyArray );

	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	
	void test_SetPtr_OnEmptyArray()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array;

		array.SetPtr( buffer, 4 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 4 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer );
	}

	void test_TwoConsecutiveSetPtrs_OnEmptyArray()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array;

		array.SetPtr( buffer, 4 );

		int buffer2[] = { 1, 2, 3, 4, 5, 6 };

		array.SetPtr( buffer2, 6 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 6 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer2 );

	}

	void test_SetPtr_OnNonEmptyArray()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array;
		array.Resize( 4 );
		array.SetSize( 4 );


		try
		{
			array.SetPtr( buffer, 4 );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL( "Array::SetPtr() on Memory Owning, non-empty Array must raise and assertion failure" );

	}

	void testArray_Constructing_NonMemoryOwning_With_Valid_Pointer()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array( buffer, 4 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 4 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer );

	}


	void testArray_Constructing_NonMemoryOwning_With_Null_Pointer()
	{
		try
		{
			CLAM::Array<int> array( (int*)NULL, 0 );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL("Invoking the Array::Array( T*, int) constructor passing a "
			     "NULL pointer should have triggered an assertion failure!");
	}


	void testCopy_MemoryOwning_Into_MemoryOwning()
	{
		CLAM::DataArray src;
		src.Resize( 10 );
		src.SetSize( 10 );

		for ( int i = 0; i < src.Size(); i++ )
			src[i] = i;

		CLAM::DataArray dst;
		dst.Resize( 10 );
		dst.SetSize( 10 );

		dst = src;

		bool sizeItsTheSame = ( src.Size() == dst.Size() );
		bool dstDoesOwnMemory = dst.OwnsMemory();
		bool allElemsEqual = true;
		
		for ( int i = 0; i < dst.Size(); i++ )
		{
			allElemsEqual &= ( dst[i] == i );
		}
		
		CPPUNIT_ASSERT( allElemsEqual );
		CPPUNIT_ASSERT( sizeItsTheSame );
		CPPUNIT_ASSERT( dstDoesOwnMemory );


	}

	void testCopy_Being_The_Array_The_Placeholder_For_A_WriteStreamRegion()
	{

		CLAM::Array<int> streamBuffer;
		streamBuffer.Resize(4);
		streamBuffer.SetSize(4);

		streamBuffer[0] = 1;
		streamBuffer[1] = 2;
		streamBuffer[2] = 3;
		streamBuffer[3] = 4;
		


		CLAM::Array<int> array;		
		streamBuffer.GiveChunk( 1, 2, array );

		CLAM::Array<int> data;
		data.Resize(2);
		data.SetSize(2);

		array = data;


		CPPUNIT_ASSERT( streamBuffer[0] == 1 );
		CPPUNIT_ASSERT( streamBuffer[1] == 0 );
		CPPUNIT_ASSERT( streamBuffer[2] == 0 );
		CPPUNIT_ASSERT( streamBuffer[3] == 4 );

		
	}

};






} // namespace CLAMTest
