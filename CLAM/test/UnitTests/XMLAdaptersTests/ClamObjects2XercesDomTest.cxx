#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

//#include "XercesDomWriter.hxx"
//#include "XercesEncodings.hxx"
//#include <xercesc/dom/DOMElement.hpp>
//#include <xercesc/dom/DOMText.hpp>
#include "ClamObjects2XercesDom.hxx"
#include "BasicXMLable.hxx"
#include "Component.hxx"
#include <list>
#include "XercesDomWriter.hxx"
#include <xercesc/dom/DOMElement.hpp>

namespace CLAM
{
namespace Test
{

class MockUpBasic : public BasicXMLable
{
public:
	MockUpBasic(const char * name=0, bool isElement=false)
		: BasicXMLable(name, isElement) {}
	void setContent(const std::string & content)
	{
		_content = content;	
	}
	std::string XMLContent() const
	{
		return _content;
	}

       //* Extracts the content from the stream.
	bool XMLContent(std::istream & str)
	{
		str >> _content;
		return str!=NULL;
	}
private:
	std::string _content;
};

class MockUpComponent : public MockUpBasic, public Component {
	std::list<XMLable *> _parts;
public:
	MockUpComponent(const char * name=0, bool isElement=false)
		: MockUpBasic(name, isElement) {}
	void add(XMLable & part)
	{
		_parts.push_back(&part);
	}
	const char * GetClassName() const { return "CLAMTest::MockUpComponent";}
	void StoreOn(Storage & store)
	{
	}
	void LoadFrom(Storage & store)
	{
	}
};

class ClamObjectsToXercesDom;

CPPUNIT_TEST_SUITE_REGISTRATION( ClamObjectsToXercesDom );

class ClamObjectsToXercesDom : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( ClamObjectsToXercesDom );
	CPPUNIT_TEST(testEmptyDocument);
	CPPUNIT_TEST(testBasicAsPlainContent);
	CPPUNIT_TEST(testBasicAsElement_withoutContent);
	CPPUNIT_TEST(testBasicAsElement_withContent);
	CPPUNIT_TEST(testBasicAsPlainContent_withoutContent);
	CPPUNIT_TEST(testBasicAsAttribute);
	CPPUNIT_TEST(testComponentAsPlainContent);
	CPPUNIT_TEST(testComponentAsElement_withoutContent);
	CPPUNIT_TEST(testComponentAsElement_withContent);
	CPPUNIT_TEST(testComponentAsPlainContent_withoutContent);
	CPPUNIT_TEST(testComponentAsAttribute);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}

private:
	std::stringstream mTargetStream;

	void testEmptyDocument()
	{
		ClamObject2XercesDom dumper("Doc");
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc/>"), mTargetStream.str());
	}

	void testBasicAsPlainContent()
	{
		MockUpBasic basic;
		basic.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content</Doc>"), mTargetStream.str());
	}

	void testBasicAsPlainContent_withoutContent()
	{
		MockUpBasic basic;
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc/>"), mTargetStream.str());
	}

	void testBasicAsElement_withoutContent()
	{
		MockUpBasic basic("Element",true);
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element/></Doc>"), mTargetStream.str());
	}

	void testBasicAsElement_withContent()
	{
		MockUpBasic basic("Element",true);
		basic.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element></Doc>"), mTargetStream.str());
	}
	
	void testBasicAsAttribute()
	{
		MockUpBasic basic("at",false);
		basic.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc at=\"Content\"/>"), mTargetStream.str());
	}

	void testComponentAsPlainContent()
	{
		MockUpComponent component;
		component.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content</Doc>"), mTargetStream.str());
	}

	void testComponentAsPlainContent_withoutContent()
	{
		MockUpComponent component;
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc/>"), mTargetStream.str());
	}

	void testComponentAsElement_withoutContent()
	{
		MockUpComponent component("Element",true);
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element/></Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_withContent()
	{
		MockUpComponent component("Element",true);
		component.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element></Doc>"), mTargetStream.str());
	}
	
	void testComponentAsAttribute()
	{
		MockUpComponent component("at",false);
		component.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc at=\"Content\"/>"), mTargetStream.str());
	}

	void testComponentAsElementContent()
	{
	}

	void testInsideBasicAsPlainContent()
	{
	}

	void testInsideBasicAsElement()
	{
	}

	void testInsideBasicAsAttribute()
	{
	}

	void testInsideElementAsAttribute()
	{
	}

};




} // namespace Test
} // namespace Cuidado

