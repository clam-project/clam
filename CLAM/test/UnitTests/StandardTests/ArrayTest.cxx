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
	CPPUNIT_TEST( testCreate_Array_As_NonMemoryOwning );
	CPPUNIT_TEST( testCopy_MemoryOwning_Into_MemoryOwning );
	CPPUNIT_TEST( testCopy_MemoryOwning_Into_NonMemoryOwning );
	//CPPUNIT_TEST( testCopy_NonMemoryOwning_Into_MemoryOwning );
	//CPPUNIT_TEST( testCopy_NonMemoryOwning_Into_NonMemoryOwning );
	CPPUNIT_TEST( test_SetPtr_OnNonMemoryOwning_without_SizeExpectations );
	CPPUNIT_TEST( test_TwoSetPtrs_OnTheSameNonMemoryOwning_without_SizeExpectations );
	CPPUNIT_TEST( test_SetPtr_OnNonMemoryOwning_with_SizeExpectations );
	CPPUNIT_TEST( test_TwoSetPtrs_OnTheSameNonMemoryOwning_with_SizeExpectations );
	CPPUNIT_TEST( test_SetPtr_OnMemoryOwningNonEmpty );
	
	CPPUNIT_TEST( test_SetPtr_OnEmptyArray );
	CPPUNIT_TEST( test_TwoSetPtrs_OnEmptyArray );

	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	
	void test_SetPtr_OnNonMemoryOwning_without_SizeExpectations()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array( (int*)NULL );

		array.SetPtr( buffer, 4 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 4 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer );
	}

	void test_TwoSetPtrs_OnTheSameNonMemoryOwning_without_SizeExpectations()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array( (int*)NULL );

		array.SetPtr( buffer, 4 );

		int buffer2[] = { 1, 2, 3, 4, 5, 6 };

		array.SetPtr( buffer2, 6 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 6 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer2 );

	}

	void test_SetPtr_OnNonMemoryOwning_with_SizeExpectations()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array( (int*)NULL, 4 );

		array.SetPtr( buffer, 4 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 4 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer );
	}

	void test_TwoSetPtrs_OnTheSameNonMemoryOwning_with_SizeExpectations()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array( (int*)NULL, 4 );

		array.SetPtr( buffer, 4 );

		int buffer2[] = { 1, 2, 3, 4, 5, 6 };

		array.SetPtr( buffer2, 6 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 6 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer2 );

	}


	void test_SetPtr_OnMemoryOwningNonEmpty()
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

	void test_SetPtr_OnEmptyArray()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array;

		array.SetPtr( buffer, 4 );
		
		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 4 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer );

	}

	void test_TwoSetPtrs_OnEmptyArray()
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

	void testCreate_Array_As_NonMemoryOwning()
	{
		CLAM::Array<int> array( (int*)NULL, 0 );

		CPPUNIT_ASSERT_EQUAL( array.OwnsMemory(), false );
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

	void testCopy_MemoryOwning_Into_NonMemoryOwning()
	{
		CLAM::DataArray src;
		src.Resize( 10 );
		src.SetSize( 10 );

		for ( int i = 0; i < src.Size(); i++ )
			src[i] = i;

		CLAM::DataArray dst( (CLAM::TData*)NULL, src.Size() );

		dst = src;

		bool sizeItsTheSame = ( src.Size() == dst.Size() );
		bool dstDoesNotOwnMemory = !dst.OwnsMemory();
		bool pointersMatch = ( src.GetPtr() == dst.GetPtr() );
		bool allElemsEqual = true;
		
		for ( int i = 0; i < dst.Size(); i++ )
		{
			allElemsEqual &= ( dst[i] == i );
		}

		CPPUNIT_ASSERT( allElemsEqual );
		CPPUNIT_ASSERT( sizeItsTheSame );
		CPPUNIT_ASSERT( dstDoesNotOwnMemory );
		CPPUNIT_ASSERT( pointersMatch );
		CPPUNIT_ASSERT( allElemsEqual );
		
	}

	void testCopy_NonMemoryOwning_Into_MemoryOwning()
	{

		CLAM::DataArray src;
		src.Resize( 10 );
		src.SetSize( 10 );

		for ( int i = 0; i < src.Size(); i++ )
			src[i] = i;

		CLAM::DataArray dst( (CLAM::TData*)NULL, src.Size() );

		dst = src;

		CLAM::DataArray realDst;
		realDst.Resize( dst.Size() );
		realDst.SetSize( dst.Size() );

		realDst = dst;
		
		bool sizeItsTheSame = ( src.Size() == realDst.Size() );
		bool dstDoesOwnMemory = realDst.OwnsMemory();
		bool allElemsEqual = true;
		
		for ( int i = 0; i < realDst.Size(); i++ )
		{
			allElemsEqual &= ( realDst[i] == i );
		}

		CPPUNIT_ASSERT( allElemsEqual );
		CPPUNIT_ASSERT( sizeItsTheSame );
		CPPUNIT_ASSERT( dstDoesOwnMemory );
		CPPUNIT_ASSERT( allElemsEqual );
		
	}

	void testCopy_NonMemoryOwning_Into_NonMemoryOwning()
	{
		CLAM::DataArray src;
		src.Resize( 10 );
		src.SetSize( 10 );

		for ( int i = 0; i < src.Size(); i++ )
			src[i] = i;

		CLAM::DataArray dst( (CLAM::TData*)NULL, src.Size() );

		dst = src;

		CLAM::DataArray realDst( (CLAM::TData*)NULL, src.Size() );

		realDst = dst;
		
		bool sizeItsTheSame = ( src.Size() == realDst.Size() );
		bool dstDoesNotOwnMemory = !realDst.OwnsMemory();
		bool pointersMatch = ( dst.GetPtr() == realDst.GetPtr());
		bool allElemsEqual = true;
		
		for ( int i = 0; i < realDst.Size(); i++ )
		{
			allElemsEqual &= ( realDst[i] == i );
		}

		CPPUNIT_ASSERT( allElemsEqual );
		CPPUNIT_ASSERT( sizeItsTheSame );
		CPPUNIT_ASSERT( dstDoesNotOwnMemory );
		CPPUNIT_ASSERT( pointersMatch );
		CPPUNIT_ASSERT( allElemsEqual );

	}
};






} // namespace CLAMTest
