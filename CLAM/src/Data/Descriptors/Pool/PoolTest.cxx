#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{

class PoolTest;

CPPUNIT_TEST_SUITE_REGISTRATION( PoolTest );

class PoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PoolTest );
	CPPUNIT_TEST( testGetIndex_whenEmpty );
	CPPUNIT_TEST( testGetIndex_withOneInserted );
	CPPUNIT_TEST( testGetIndex_withAWrongName );
	CPPUNIT_TEST( testGetIndex_withSecondInsertedArray );
	CPPUNIT_TEST( testGet_ReturnsSameMemory );
	CPPUNIT_TEST( testGet_ReturnsConstMemory );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

	CLAM::PoolSpec _spec;

private:
	void assertArrayEquals(unsigned size, CLAM::TData * expected, CLAM::TData * result)
	{
		for (unsigned int i = 0; i<size; i++)
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i],result[i],.0001);

	}

	void testGetIndex_whenEmpty()
	{
		std::string expected = "Not such descriptor name on this descriptor scope";
		CLAM::PoolSpec spec;
		try
		{
			spec.GetIndex("Lala");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetNAttributes());
	}

	void testGetIndex_withOneInserted()
	{
		CLAM::PoolSpec spec;
		spec.Add("Lala");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("Lala"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testGetIndex_withAWrongName()
	{
		std::string expected = "Not such descriptor name on this descriptor scope";
		CLAM::PoolSpec spec;
		spec.Add("Lala");
		try
		{
			spec.GetIndex("Foo");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testGetIndex_withSecondInsertedArray()
	{
		CLAM::PoolSpec spec;
		spec.Add("Lala");
		spec.Add("Foo");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("Lala"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("Foo"));
		CPPUNIT_ASSERT_EQUAL(2u,spec.GetNAttributes());
	}

	void testGet_ReturnsSameMemory()
	{
		const unsigned poolSize=5;
		CLAM::PoolSpec spec;
		spec.Add("Lala");
		
		CLAM::Pool pool(spec,poolSize);
		CLAM::TData * data = pool.Get("Lala");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.Get("Lala");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testGet_ReturnsConstMemory()
	{
		const unsigned poolSize=5;
		CLAM::PoolSpec spec;
		spec.Add("Lala");

		CLAM::Pool pool(spec,poolSize);
		CLAM::TData * data = pool.Get("Lala");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		const CLAM::Pool & pool2 = pool;
		const CLAM::TData * data2 = pool2.Get("Lala");
		CPPUNIT_ASSERT_EQUAL(const_cast<const CLAM::TData*>(data),data2);
	}

	



};






} // namespace CLAMTest
