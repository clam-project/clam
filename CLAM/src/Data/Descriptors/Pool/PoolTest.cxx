#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{

class PoolTest;
class PoolSpecTest;

CPPUNIT_TEST_SUITE_REGISTRATION( PoolSpecTest );
CPPUNIT_TEST_SUITE_REGISTRATION( PoolTest );

class PoolSpecTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PoolSpecTest );
	CPPUNIT_TEST( testGetIndex_whenEmpty );
	CPPUNIT_TEST( testGetIndex_withOneInserted );
	CPPUNIT_TEST( testGetIndex_withAWrongName );
	CPPUNIT_TEST( testGetIndex_withSecondInsertedArray );
	CPPUNIT_TEST( testAddAttribute_whenNameAlreadyAdded );
	CPPUNIT_TEST( testAdding_DifferentTypes );
	CPPUNIT_TEST( testCheckType_withOtherType );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
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
		spec.Add<CLAM::TData>("Lala");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("Lala"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testGetIndex_withAWrongName()
	{
		std::string expected = "Not such descriptor name on this descriptor scope";
		CLAM::PoolSpec spec;
		spec.Add<CLAM::TData>("Lala");
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
		spec.Add<CLAM::TData>("Lala");
		spec.Add<CLAM::TData>("Foo");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("Lala"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("Foo"));
		CPPUNIT_ASSERT_EQUAL(2u,spec.GetNAttributes());
	}

	void testAddAttribute_whenNameAlreadyAdded()
	{
		std::string expected = "ScopeSpec::Add, Attribute already present";
		CLAM::PoolSpec spec;
		spec.Add<CLAM::TData>("Lala");
		try
		{
			spec.Add<CLAM::TData>("Lala");
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

	void testAdding_DifferentTypes()
	{
		CLAM::PoolSpec spec;
		spec.Add<CLAM::TData>("Lala");
		spec.Add<CLAM::TIndex>("Foo");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("Lala"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("Foo"));
		CPPUNIT_ASSERT_EQUAL(2u,spec.GetNAttributes());
	}

	void testCheckType_withOtherType()
	{
		std::string expected = "Type Missmatch using a pool";
		CLAM::PoolSpec spec;
		spec.Add<CLAM::TData>("Lala");
		try
		{
			spec.CheckType(1,(CLAM::TIndex*)0);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
	}

};





class PoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PoolTest );
	CPPUNIT_TEST( testGet_ReturnsSameMemory );
	CPPUNIT_TEST( testGet_ReturnsConstMemory );
	CPPUNIT_TEST( testGet_withStrings );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void assertArrayEquals(unsigned size, CLAM::TData * expected, CLAM::TData * result)
	{
		for (unsigned int i = 0; i<size; i++)
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i],result[i],.0001);

	}

	void testGet_ReturnsSameMemory()
	{
		const unsigned poolSize=5;
		CLAM::PoolSpec spec;
		spec.Add<CLAM::TData>("Lala");
		
		CLAM::Pool pool(spec,poolSize);
		CLAM::TData * data = pool.Get<CLAM::TData>("Lala");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.Get<CLAM::TData>("Lala");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testGet_ReturnsConstMemory()
	{
		const unsigned poolSize=5;
		CLAM::PoolSpec spec;
		spec.Add<CLAM::TData>("Lala");

		CLAM::Pool pool(spec,poolSize);
		CLAM::TData * data = pool.Get<CLAM::TData>("Lala");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		const CLAM::Pool & pool2 = pool;
		const CLAM::TData * data2 = pool2.Get<CLAM::TData>("Lala");
		CPPUNIT_ASSERT_EQUAL(const_cast<const CLAM::TData*>(data),data2);
	}

	void testGet_withStrings()
	{
		const unsigned poolSize=5;
		CLAM::PoolSpec spec;
		spec.Add<std::string>("Lala");

		CLAM::Pool pool(spec,poolSize);
		std::string * data = pool.Get<std::string>("Lala");
		for (unsigned i = 0; i < poolSize; i++)
		{
			std::ostringstream os;
			os << "Hola " << i*i;
			data[i] += os.str();
		}
		const CLAM::Pool & pool2 = pool;
		const std::string * data2 = pool2.Get<std::string>("Lala");
		const std::string expected = "Hola 16";
		CPPUNIT_ASSERT_EQUAL(expected,data2[4]);
	}

};






} // namespace CLAMTest
