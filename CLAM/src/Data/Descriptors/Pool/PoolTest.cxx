#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"
#include "Component.hxx"


namespace CLAMTest
{

class ScopePoolTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ScopePoolTest );

class ScopePoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ScopePoolTest );
	CPPUNIT_TEST( testGet_ReturnsSameMemory );
	CPPUNIT_TEST( testGet_ReturnsConstMemory );
	CPPUNIT_TEST( testGet_withStrings );
	CPPUNIT_TEST( testGet_withWrongType );
	CPPUNIT_TEST( testGet_withWrongTypeAndConst );
	CPPUNIT_TEST( testConstruction_withoutSize );
	CPPUNIT_TEST( testConstruction_withoutSizeGettingNoConst );
	CPPUNIT_TEST( testConstruction_givesSizeZeroByDefault );
	CPPUNIT_TEST( testSetSize_overAZeroSizePool );
	CPPUNIT_TEST( testSetSize_overANonZeroSizePool );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testGet_ReturnsSameMemory()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		
		CLAM::ScopePool pool(spec,poolSize);
		CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.Get<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testGet_ReturnsConstMemory()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec,poolSize);
		CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		const CLAM::ScopePool & pool2 = pool;
		const CLAM::TData * data2 = pool2.Get<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(const_cast<const CLAM::TData*>(data),data2);
	}

	void testGet_withStrings()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec;
		spec.Add<std::string>("MyAttribute");

		CLAM::ScopePool pool(spec,poolSize);
		std::string * data = pool.Get<std::string>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
		{
			std::ostringstream os;
			os << "Hola " << i*i;
			data[i] += os.str();
		}
		const CLAM::ScopePool & pool2 = pool;
		const std::string * data2 = pool2.Get<std::string>("MyAttribute");
		const std::string expected = "Hola 16";
		CPPUNIT_ASSERT_EQUAL(expected,data2[4]);
	}

	void testGet_withWrongType()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec;
		spec.Add<std::string>("MyAttribute");

		CLAM::ScopePool pool(spec,poolSize);
		try
		{
			int * data = pool.Get<int>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Type Missmatch using a pool";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGet_withWrongTypeAndConst()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec;
		spec.Add<std::string>("MyAttribute");

		CLAM::ScopePool pool(spec,poolSize);
		const CLAM::ScopePool & pool2 = pool;
		try
		{
			const int * data = pool2.Get<int>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Type Missmatch using a pool";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testConstruction_withoutSize()
	{
		CLAM::DescriptionScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec);
		const CLAM::ScopePool & constPool = pool;

		try
		{
			const CLAM::TData * data = constPool.Get<CLAM::TData>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Getting an attribute from a zero size pool";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testConstruction_withoutSizeGettingNoConst()
	{
		CLAM::DescriptionScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec);

		try
		{
			CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Getting an attribute from a zero size pool";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testConstruction_givesSizeZeroByDefault()
	{
		CLAM::DescriptionScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec);
		CPPUNIT_ASSERT_EQUAL(0u,pool.GetSize());

	}

	void testSetSize_overAZeroSizePool()
	{
		const unsigned poolSize=5;

		CLAM::DescriptionScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec);

		pool.SetSize(poolSize);

		CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.Get<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testSetSize_overANonZeroSizePool()
	{
		const unsigned poolSize=5;

		CLAM::DescriptionScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec,7);

		pool.SetSize(poolSize);

		CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.Get<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}


};






} // namespace CLAMTest
