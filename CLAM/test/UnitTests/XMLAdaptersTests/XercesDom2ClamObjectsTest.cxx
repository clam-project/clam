#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "XercesDom2ClamObjects.hxx"
#include "XmlMockUpObjects.hxx"
#include "Component.hxx"
#include <list>
#include "XercesDomWriter.hxx"
#include "XercesInitializer.hxx"
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMComment.hpp>
#include <xercesc/dom/DOMProcessingInstruction.hpp>
/*
TOTEST:
- Return values on error conditions
- Recursive behaviour for text components
- Recursive behaviour for element components
*/

namespace CLAM
{
namespace Test
{

class XercesDomToClamObjectsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XercesDomToClamObjectsTest );

class XercesDomToClamObjectsTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XercesDomToClamObjectsTest );

	CPPUNIT_TEST(testLoadingAWordOnBasicAsContent);
	CPPUNIT_TEST(testLoadingTwoWordsOnTwoBasicsAsContent);
	CPPUNIT_TEST(testLoadingOneWordsOnTwoBasicsAsContent_secondLoadFails);
	CPPUNIT_TEST(testEmptyElement_WithRightLabel);
	CPPUNIT_TEST(testEmptyElement_WithWrongLabel);
	CPPUNIT_TEST(testLoadingElementContent);
	CPPUNIT_TEST(testLoadingContentAfterElement);
	CPPUNIT_TEST(testLoadingSubobjects);
	CPPUNIT_TEST(testLoadingSubobjects_whenComponentIsContent);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		mTargetStream.str("");
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}

private:
	std::stringstream mTargetStream;

	void testLoadingAWordOnBasicAsContent()
	{
		std::istringstream xml(
			"<Doc>"
			"Content"
			"</Doc>"
			);
		
		XmlMockUpBasic basic;
		basic.setContent("PreviousValue");

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result = loader.Load(basic);

		CPPUNIT_ASSERT_EQUAL(true, result);

		std::string expected= "B'Content'\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

	void testLoadingTwoWordsOnTwoBasicsAsContent()
	{
		std::istringstream xml(
			"<Doc>"
			"Content1 Content2"
			"</Doc>"
			);
		
		XmlMockUpBasic basic1;
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result1 = loader.Load(basic1);
		bool result2 = loader.Load(basic2);

		CPPUNIT_ASSERT_EQUAL(true, result1);
		CPPUNIT_ASSERT_EQUAL(true, result2);

		std::string expected1= "B'Content1'\n"; 
		CPPUNIT_ASSERT_EQUAL(expected1, basic1.structureTrace(0));
		std::string expected2= "B'Content2'\n"; 
		CPPUNIT_ASSERT_EQUAL(expected2, basic2.structureTrace(0));
	}

	void testLoadingOneWordsOnTwoBasicsAsContent_secondLoadFails()
	{
		std::istringstream xml(
			"<Doc>"
			"Content1"
			"</Doc>"
			);
		
		XmlMockUpBasic basic1;
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result1 = loader.Load(basic1);
		bool result2 = loader.Load(basic2);

		CPPUNIT_ASSERT_EQUAL(true, result1);
		CPPUNIT_ASSERT_EQUAL(false, result2);

		std::string expected1= "B'Content1'\n"; 
		CPPUNIT_ASSERT_EQUAL(expected1, basic1.structureTrace(0));
		std::string expected2= "B''\n"; 
		CPPUNIT_ASSERT_EQUAL(expected2, basic2.structureTrace(0));
	}

	void testEmptyElement_WithRightLabel()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element/>"
			"</Doc>"
			);

		XmlMockUpBasic basic("Element",true);

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result = loader.Load(basic);

		CPPUNIT_ASSERT_EQUAL(true, result);

		std::string expected= "B''\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}
	void testEmptyElement_WithWrongLabel()
	{
		std::istringstream xml(
			"<Doc>"
			"<Other/>"
			"</Doc>"
			);

		XmlMockUpBasic basic("Element",true);
		basic.setContent("PreviousValue");

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result = loader.Load(basic);

		CPPUNIT_ASSERT_EQUAL(false, result);

		std::string expected= "B'PreviousValue'\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

	void testLoadingElementContent()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element>ElementContent</Element>"
			"</Doc>"
			);
		
		XmlMockUpComponent component("Element",true);
		component.setContent("PreviousValue1");

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result = loader.Load(component);

		CPPUNIT_ASSERT_EQUAL(true, result);
		std::string expected= "C'ElementContent'\n{\n}\n"; 
		CPPUNIT_ASSERT_EQUAL(expected, component.structureTrace(0));
	}

	void testLoadingContentAfterElement()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element/>"
			"AfterElementContent"
			"</Doc>"
			);
		
		XmlMockUpBasic basic1("Element",true);
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result1 = loader.Load(basic1);
		bool result2 = loader.Load(basic2);

		CPPUNIT_ASSERT_EQUAL(true, result1);
		CPPUNIT_ASSERT_EQUAL(true, result2);

		std::string expected1= "B''\n"; 
		CPPUNIT_ASSERT_EQUAL(expected1, basic1.structureTrace(0));

		std::string expected2= "B'AfterElementContent'\n"; 
		CPPUNIT_ASSERT_EQUAL(expected2, basic2.structureTrace(0));
	}

	void testLoadingSubobjects()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element>"
			"<SubElement/>"
			"AfterElementContent"
			"</Element>"
			"</Doc>"
			);
		XmlMockUpComponent component("Element",true);
		component.setContent("PreviousElementContent");
		XmlMockUpBasic basic1("SubElement",true);
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");
		component.add(basic1);
		component.add(basic2);

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result = loader.Load(component);

		CPPUNIT_ASSERT_EQUAL(true, result);

		std::string expected= 
			"C''\n"
			"{\n"
			".B''\n" 
			".B'AfterElementContent'\n"
			"}\n";

		CPPUNIT_ASSERT_EQUAL(expected, component.structureTrace(0));
	}

	void testLoadingSubobjects_whenComponentIsContent()
	{
		std::istringstream xml(
			"<Doc>"
			"<SubElement/>"
			"AfterElementContent"
			"</Doc>"
			);
		XmlMockUpComponent component;
		component.setContent("PreviousElementContent");
		XmlMockUpBasic basic1("SubElement",true);
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");
		component.add(basic1);
		component.add(basic2);

		XercesDom2ClamObjects loader;
		loader.Read(xml);
		bool result = loader.Load(component);

		CPPUNIT_ASSERT_EQUAL(false, result); // because the component content is empty

		std::string expected= 
			"C''\n"
			"{\n"
			".B''\n" 
			".B'AfterElementContent'\n"
			"}\n";

		CPPUNIT_ASSERT_EQUAL(expected, component.structureTrace(0));
	}

};




} // namespace Test
} // namespace Cuidado

