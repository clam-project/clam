#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "Audio.hxx"
#include "SpectralAnalysis.hxx"
#include "XMLStorage.hxx"
#include "XMLAdapter.hxx"
#include <sstream>


namespace CLAMTest
{

class DescriptionXmlTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DescriptionXmlTest );

class DescriptionXmlTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DescriptionXmlTest );
	CPPUNIT_TEST(testDumpScopePool_withNoAttributes);
	CPPUNIT_TEST(testDumpScopePool_withAttributesAndZeroSize);
	CPPUNIT_TEST(testDumpScopePool_withAttributes);
	CPPUNIT_TEST(testDumpScopePool_withIntegerAttributes);
	CPPUNIT_TEST(testDumpScopePool_withComponentAttributes);
	CPPUNIT_TEST(testDumpScopePool_withNonInstantiatedAttributes);
	CPPUNIT_TEST(testDumpDescriptionDataPool_withAllKindsOfData);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	std::ostringstream _targetStream;
	class DummyComponent : public CLAM::Component
	{
	public:
		void SetValue(const std::string & newValue)
		{
			_dummyMember=newValue;
		}
		const char * GetClassName() const { return "DummyComponent"; }
		void StoreOn(CLAM::Storage & storage) const
		{
			CLAM::XMLAdapter<std::string> adapter(_dummyMember,"DummyMember",false);
			storage.Store(adapter);
		}
		void LoadFrom(CLAM::Storage & storage)
		{
		}
	private:
		std::string _dummyMember;
	};

	void testDumpAttributePool_withSimpleData()
	{
	}
	void testDumpAttributePool_withComponentData()
	{
	}
	void testDumpAttributePool_withNonDumpableData()
	{
	}
	void testDumpScopePool_withNoAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		CLAM::ScopePool pool(scope,20);

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"20\"/>"
			),_targetStream.str());
	}
	void testDumpScopePool_withAttributesAndZeroSize()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<std::string>("MyAttribute");
		CLAM::ScopePool pool(scope,0);

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"0\">"
			"<AttributePool name=\"MyAttribute\">"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<std::string>("MyAttribute");
		CLAM::ScopePool pool(scope,3);
		std::string * values = pool.GetWritePool<std::string>("MyAttribute");
		values[0]="value0";
		values[1]="value1";
		values[2]="value2";

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"value0 value1 value2"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withIntegerAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<unsigned>("MyAttribute");
		CLAM::ScopePool pool(scope,3);
		unsigned * values = pool.GetWritePool<unsigned>("MyAttribute");
		values[0]=1;
		values[1]=2;
		values[2]=3;

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"1 2 3"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withComponentAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<DummyComponent>("MyAttribute");
		CLAM::ScopePool pool(scope,3);
		DummyComponent * values = pool.GetWritePool<DummyComponent>("MyAttribute");
		values[0].SetValue("value0");
		values[1].SetValue("value1");
		values[2].SetValue("value2");

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"<DummyComponent DummyMember=\"value0\"/>"
				"<DummyComponent DummyMember=\"value1\"/>"
				"<DummyComponent DummyMember=\"value2\"/>"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withNonInstantiatedAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<DummyComponent>("MyAttribute");
		CLAM::ScopePool pool(scope,3);

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"3\"/>"
			),_targetStream.str());
	}
	void testDumpDescriptionDataPool_withAllKindsOfData()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute< CLAM::Attribute<DummyComponent> >("TestScope1","DummyComponentAttribute");
		scheme.AddAttribute< CLAM::Attribute<unsigned> >("TestScope2","UnsignedAttribute");
		scheme.AddAttribute< CLAM::Attribute<std::string> >("TestScope2","StringAttribute");

		CLAM::DescriptionDataPool pool(scheme);
		pool.SetNumberOfContexts("TestScope1",3);
		pool.SetNumberOfContexts("TestScope2",4);
		{
			DummyComponent * values = pool.GetAttributePool<DummyComponent>("TestScope1","DummyComponentAttribute");
			values[0].SetValue("value0");
			values[1].SetValue("value1");
			values[2].SetValue("value2");
		}
		{
			std::string * values = pool.GetAttributePool<std::string>("TestScope2","StringAttribute");
			values[0]="value0";
			values[1]="value1";
			values[2]="value2";
			values[3]="value3";
		}
		{
			unsigned * values = pool.GetAttributePool<unsigned>("TestScope2","UnsignedAttribute");
			values[0]=0;
			values[1]=1;
			values[2]=2;
			values[3]=3;
		}

		CLAM::XmlStorage::Dump(pool,"DescriptionData",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
		"<DescriptionData>"
			"<ScopePool name=\"TestScope1\" size=\"3\">"
			"<AttributePool name=\"DummyComponentAttribute\">"
				"<DummyComponent DummyMember=\"value0\"/>"
				"<DummyComponent DummyMember=\"value1\"/>"
				"<DummyComponent DummyMember=\"value2\"/>"
			"</AttributePool>"
			"</ScopePool>"
			"<ScopePool name=\"TestScope2\" size=\"4\">"
			"<AttributePool name=\"UnsignedAttribute\">"
				"0 1 2 3"
			"</AttributePool>"
			"<AttributePool name=\"StringAttribute\">"
				"value0 value1 value2 value3"
			"</AttributePool>"
			"</ScopePool>"
		"</DescriptionData>"
			),_targetStream.str());
	}

};


} // namespace CLAMTest
