#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "XercesDom2ClamObjects.hxx"
#include "XmlMockUpObjects.hxx"
#include "Component.hxx"
#include <list>
#include "XercesDomWriter.hxx"
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMComment.hpp>
#include <xercesc/dom/DOMProcessingInstruction.hpp>
/*
TOTEST:
- A comment doesn't break content
- Any non-comment node does break content
- Spaces at the begining of content are eaten
- Attributes support
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

	CPPUNIT_TEST(testFetchContent_withASingleWordContent);
	CPPUNIT_TEST(testFetchContent_withTwoJointContents);
	CPPUNIT_TEST(testFetchContent_withTwoContentsSeparatedByElement);
	CPPUNIT_TEST(testFetchContent_withTwoContentsSeparatedByComment);
	CPPUNIT_TEST(testFetchContent_withNoNode);
	CPPUNIT_TEST(testFetchContent_withElementFirst);
	CPPUNIT_TEST(testContentLeft_withNoNode_returnsFalse);
	CPPUNIT_TEST(testContentLeft_withNonSpaceReturnsTrue);
	CPPUNIT_TEST(testContentLeft_withStartingSpacesAndNonSpace_chopsAndReturnsTrue);
	CPPUNIT_TEST(testContentLeft_withOnlySpaces_chopsAndReturnsFalse);
	CPPUNIT_TEST(testFindElement_withThatElementFirst);
	CPPUNIT_TEST(testFindElement_withWrongNameFails);
	CPPUNIT_TEST(testFindElement_withCommentBefore);
	CPPUNIT_TEST(testFindElement_withNoElement);
	CPPUNIT_TEST(testFindElement_withTextFirst);
	CPPUNIT_TEST(testFindElement_withReadedTextFirst);
	CPPUNIT_TEST(testFindElement_withHalfReadedTextFirst);
	CPPUNIT_TEST(testFindElement_withSpacesToReadFirst);
	CPPUNIT_TEST(testFindElement_withStillNonElement_asserts);
	CPPUNIT_TEST(testFetchElement_withThatElementFirst);
	CPPUNIT_TEST(testFetchElement_withSecondElementFirst);
	CPPUNIT_TEST(testFetchElement_withTextFirst);
	CPPUNIT_TEST(testFetchElement_withNoElement);
	CPPUNIT_TEST(testFetchElement_withADifferentName);
	CPPUNIT_TEST(testFetchElement_withANonElementNode);
	CPPUNIT_TEST(testFetchContent_afterElement);
	CPPUNIT_TEST(testFetchContent_whenSecondElement);
	CPPUNIT_TEST(testFetchContent_afterElementWithSpaces);
	CPPUNIT_TEST(testReleaseContext_atRootReturnsNull);
	CPPUNIT_TEST(testReleaseContext_whenIsChildContext);
	CPPUNIT_TEST(testRecursiveConstructor_initializesTheContext);


	CPPUNIT_TEST(testLoadingAWordOnBasicAsContent);
	CPPUNIT_TEST(testLoadingTwoWordsOnTwoBasicsAsContent);
	CPPUNIT_TEST(testLoadingOneWordsOnTwoBasicsAsContent_secondLoadFails);
	CPPUNIT_TEST(testLoadingSplittedPlainContent_getJoined);
//	CPPUNIT_TEST(testEmptyElement_WithRightLabel);
//	CPPUNIT_TEST(testEmptyElement_WithWrongLabel);
	// TODO: All those tests are useless!!
#if 0
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
	CPPUNIT_TEST(testSibblingsContentsAndAttributes_getOrderedAsInserted);
	CPPUNIT_TEST(testConsecutiveContents_getSpaceSeparation);
	CPPUNIT_TEST(testNonConsecutiveContents_dontGetSpaceSeparation);
	CPPUNIT_TEST(testSibblingsAttributes_getReordered);
	CPPUNIT_TEST(testComponentAsElement_containingBasicAsPlainContent);
	CPPUNIT_TEST(testElementContents_getPrintedBeforeSiblingContent);
	CPPUNIT_TEST(testNodesInsertionAfterComponentElement);
	CPPUNIT_TEST(testComponentAsElement_containingBasicAsAttribute);
	CPPUNIT_TEST(testComponentAsElement_containingBasicAsElement);
	CPPUNIT_TEST(testComponentAsElement_containingComponentAsElement);
	CPPUNIT_TEST(testComponentAsContent_containingBasicAsPlainContent);
	CPPUNIT_TEST(testComponentAsContent_containingBasicAsAttribute);
	CPPUNIT_TEST(testComponentAsContent_containingBasicAsElement);
	CPPUNIT_TEST(testComponentAsAttribute_containingAnything_childrenHaveNoEffect);
#endif
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		xercesc::XMLPlatformUtils::Initialize();
		mTargetStream.str("");
		xercesc::DOMImplementation * imp = 
			xercesc::DOMImplementation::getImplementation();
		mDocument = imp->createDocument(
			X("2003-04.clam05.iua.mtg.upf.es"), // root element namespace URI.
			X("TestDoc"), // root element name
			0  // document type object (DTD).
		);
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
		mDocument->release();
		xercesc::XMLPlatformUtils::Terminate();
	}

private:
	std::stringstream mTargetStream;
	xercesc::DOMDocument * mDocument;

	void testFetchContent_withASingleWordContent()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		std::string content;
		std::getline(context.reachableContent(), content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}

	void testFetchContent_withTwoJointContents()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		contextElement->appendChild(domContent1);
		contextElement->appendChild(domContent2);

		XercesDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1Content2"),content);
	}

	void testFetchContent_withTwoContentsSeparatedByElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMElement * separator = mDocument->createElement(X("Separator"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		contextElement->appendChild(domContent1);
		contextElement->appendChild(separator);
		contextElement->appendChild(domContent2);

		XercesDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1"),content);
	}

	void testFetchContent_withTwoContentsSeparatedByComment()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMComment * separator = mDocument->createComment(X("Separator"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		contextElement->appendChild(domContent1);
		contextElement->appendChild(separator);
		contextElement->appendChild(domContent2);

		XercesDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1Content2"),content);
	}

	void testFetchContent_withNoNode()
	{

		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		XercesDomReadingContext context(contextElement);
		std::string content="rubbish";

		std::istream & stream = context.reachableContent();
		
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(true,stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}

	void testFetchContent_withElementFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * unexpectedElement = mDocument->createElement(X("UnexpectedElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		contextElement->appendChild(unexpectedElement);
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		
		std::getline(stream, content);
		CPPUNIT_ASSERT(stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}

	void testContentLeft_withNoNode_returnsFalse()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));

		XercesDomReadingContext context(contextElement);
		std::string content="rubbish";

		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(!charactersLeft);
	}

	void testContentLeft_withNonSpaceReturnsTrue()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}

	void testContentLeft_withStartingSpacesAndNonSpace_chopsAndReturnsTrue()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("  \t  \n Content"));
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}

	void testContentLeft_withOnlySpaces_chopsAndReturnsFalse()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X(" \t \n  "));
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(!charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}

	void testFindElement_withThatElementFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}

	void testFindElement_withWrongNameFails()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement1 = mDocument->createElement(X("Wrong"));
		xercesc::DOMElement * domElement2 = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement1);
		contextElement->appendChild(domElement2);

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}

	void testFindElement_withCommentBefore()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMComment * separator = mDocument->createComment(X("Separator"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(separator);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}

	void testFindElement_withNoElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}

	void testFindElement_withTextFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("ContentLeft"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}

	void testFindElement_withReadedTextFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("ReadingContent"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		std::string content;
		context.reachableContent() >> content;

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
		CPPUNIT_ASSERT_EQUAL(std::string("ReadingContent"), content);
	}

	void testFindElement_withHalfReadedTextFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Reading Content"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		std::string content;
		context.reachableContent() >> content;

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
		CPPUNIT_ASSERT_EQUAL(std::string("Reading"), content);
	}

	void testFindElement_withSpacesToReadFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("    \n \t \t \r "));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}

	void testFindElement_withStillNonElement_asserts()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMProcessingInstruction * domProcessingInstruction = 
			mDocument->createProcessingInstruction(X("ProcessingInstruction"),X("Content"));
		contextElement->appendChild(domProcessingInstruction);

		XercesDomReadingContext context(contextElement);

		try
		{
			bool foundElement = context.findElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
		}
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Can't change the context to a non element node"),
				std::string(e.what()));
		}
	}

	void testFetchElement_withThatElementFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		xercesc::DOMElement * foundElement = context.fetchElement("Element");
		CPPUNIT_ASSERT_EQUAL(domElement, foundElement);
	}

	void testFetchElement_withSecondElementFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement1 = mDocument->createElement(X("Element1"));
		xercesc::DOMElement * domElement2 = mDocument->createElement(X("Element2"));
		contextElement->appendChild(domElement1);
		contextElement->appendChild(domElement2);

		XercesDomReadingContext context(contextElement);
		xercesc::DOMElement * foundElement1 = context.fetchElement("Element1");
		xercesc::DOMElement * foundElement2 = context.fetchElement("Element2");
		CPPUNIT_ASSERT_EQUAL(domElement2, foundElement2);
		CPPUNIT_ASSERT_EQUAL(domElement1, foundElement1);
	}

	void testFetchElement_withTextFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("ContentLeft"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		try 
		{
			xercesc::DOMElement * foundElement = context.fetchElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Fetching element with content left"),
				std::string(e.what()));
		}
	}

	void testFetchElement_withNoElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		xercesc::DOMElement * foundElement1 = context.fetchElement("Element");
		try 
		{
			xercesc::DOMElement * foundElement2 = context.fetchElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Accessing beyond DOM nodes"),
				std::string(e.what()));
		}
	}

	void testFetchElement_withADifferentName()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		try 
		{
			xercesc::DOMElement * foundElement = context.fetchElement("WrongElement");
			CPPUNIT_FAIL("Should have failed an assertion");
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("XML element name should be the one expected"),
				std::string(e.what()));
		}
	}
	
	void testFetchElement_withANonElementNode()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMProcessingInstruction * domProcessingInstruction = 
			mDocument->createProcessingInstruction(X("ProcessingInstruction"),X("Content"));
		contextElement->appendChild(domProcessingInstruction);

		XercesDomReadingContext context(contextElement);

		try 
		{
			xercesc::DOMElement * foundElement = context.fetchElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Can't change the context to a non element node"),
				std::string(e.what()));
		}
	}


	void testFetchContent_afterElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		contextElement->appendChild(domContent1);
		contextElement->appendChild(domElement);
		contextElement->appendChild(domContent2);

		XercesDomReadingContext context(contextElement);
		std::string content1="rubbish";
		std::getline(context.reachableContent(), content1);
		xercesc::DOMElement * foundElement = context.fetchElement("Element");
		std::string content2="rubbish";
		std::getline(context.reachableContent(), content2);

		CPPUNIT_ASSERT_EQUAL(std::string("Content2"),content2);
	}

	void testFetchContent_whenSecondElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement1 = mDocument->createElement(X("Element1"));
		xercesc::DOMElement * domElement2 = mDocument->createElement(X("Element2"));
		contextElement->appendChild(domElement1);
		contextElement->appendChild(domElement2);

		XercesDomReadingContext context(contextElement);
		xercesc::DOMElement * foundElement = context.fetchElement("Element1");
		std::istream & stream = context.reachableContent();
		std::string content="rubbish";
		std::getline(stream, content);

		CPPUNIT_ASSERT_EQUAL(true,stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}

	void testFetchContent_afterElementWithSpaces()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("  Content"));
		contextElement->appendChild(domElement);
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		xercesc::DOMElement * foundElement = context.fetchElement("Element");
		std::string content2="rubbish";
		std::getline(context.reachableContent(), content2);

		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content2);
	}

	void testReleaseContext_atRootReturnsNull()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));

		XercesDomReadingContext context(contextElement);
		XercesDomReadingContext * previous = context.release();
		
		CPPUNIT_ASSERT_EQUAL((XercesDomReadingContext*)0, previous);
	}

	void testReleaseContext_whenIsChildContext()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);
		XercesDomReadingContext context1(contextElement);
		XercesDomReadingContext context2(&context1,"Element");

		XercesDomReadingContext * previous = context2.release();
		
		CPPUNIT_ASSERT_EQUAL(&context1, previous);
	}


	void testRecursiveConstructor_initializesTheContext()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * outerElement = mDocument->createElement(X("Element"));
		xercesc::DOMElement * innerElement = mDocument->createElement(X("InnerElement"));
		contextElement->appendChild(outerElement);
		outerElement->appendChild(innerElement);
		XercesDomReadingContext contextRoot(contextElement);
		XercesDomReadingContext contextInner(&contextRoot,"Element");
		xercesc::DOMElement * foundElement = contextInner.fetchElement("InnerElemen");

		CPPUNIT_ASSERT_EQUAL(innerElement, foundElement);
	}

	void testReleaseContext_whenContentLeft()
	{
	}

	void testReleaseContext_whenElementLeft()
	{
	}






	
	void testLoadingAWordOnBasicAsContent()
	{
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		mDocument->getDocumentElement()->appendChild(domContent);
		
		XmlMockUpBasic basic;
		basic.setContent("PreviousValue");

		XercesDom2ClamObjects loader;
		loader.setDocument(mDocument);
		bool result = loader.Load(basic);

		CPPUNIT_ASSERT_EQUAL(true, result);

		std::string expected= "B'Content'\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

	void testLoadingTwoWordsOnTwoBasicsAsContent()
	{
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content1 Content2"));
		mDocument->getDocumentElement()->appendChild(domContent);
		
		XmlMockUpBasic basic1;
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");

		XercesDom2ClamObjects loader;
		loader.setDocument(mDocument);
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
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content1"));
		mDocument->getDocumentElement()->appendChild(domContent);
		
		XmlMockUpBasic basic1;
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");

		XercesDom2ClamObjects loader;
		loader.setDocument(mDocument);
		bool result1 = loader.Load(basic1);
		bool result2 = loader.Load(basic2);

		CPPUNIT_ASSERT_EQUAL(true, result1);
		CPPUNIT_ASSERT_EQUAL(false, result2);

		std::string expected1= "B'Content1'\n"; 
		CPPUNIT_ASSERT_EQUAL(expected1, basic1.structureTrace(0));
		std::string expected2= "B'PreviousValue2'\n"; 
		CPPUNIT_ASSERT_EQUAL(expected2, basic2.structureTrace(0));
	}

	void testLoadingSplittedPlainContent_getJoined()
	{
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		mDocument->getDocumentElement()->appendChild(domContent1);
		mDocument->getDocumentElement()->appendChild(domContent2);
		
		XmlMockUpBasic basic;
		basic.setContent("PreviousValue");

		XercesDom2ClamObjects loader;
		loader.setDocument(mDocument);
		bool result = loader.Load(basic);

		CPPUNIT_ASSERT_EQUAL(true, result);

		std::string expected= "B'Content1Content2'\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

	void testEmptyElement_WithRightLabel()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		
		XmlMockUpBasic basic("Element",true);
		basic.setContent("PreviousValue");

		XercesDom2ClamObjects loader;
		loader.setDocument(mDocument);
		bool result = loader.Load(basic);

		CPPUNIT_ASSERT_EQUAL(true, result);

		std::string expected= "B'PreviousValue'\n{\n}\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}
	void testEmptyElement_WithWrongLabel()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Other"));
		
		XmlMockUpBasic basic("Element",true);
		basic.setContent("PreviousValue");

		XercesDom2ClamObjects loader;
		loader.setDocument(mDocument);
		bool result = loader.Load(basic);

		CPPUNIT_ASSERT_EQUAL(false, result);

		std::string expected= "B'PreviousValue'\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}
#if 0

	void testBasicAsPlainContent()
	{
		XmlMockUpBasic basic;
		basic.setContent("Content");
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content</Doc>"), mTargetStream.str());
	}

	void testBasicAsPlainContent_withoutContent()
	{
		XmlMockUpBasic basic;
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc/>"), mTargetStream.str());
	}

	void testBasicAsElement_withoutContent()
	{
		XmlMockUpBasic basic("Element",true);
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element/></Doc>"), mTargetStream.str());
	}

	void testBasicAsElement_withContent()
	{
		XmlMockUpBasic basic("Element",true);
		basic.setContent("Content");
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element></Doc>"), mTargetStream.str());
	}
	
	void testBasicAsAttribute()
	{
		XmlMockUpBasic basic("at",false);
		basic.setContent("Content");
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc at=\"Content\"/>"), mTargetStream.str());
	}

	void testComponentAsPlainContent()
	{
		XmlMockUpComponent component;
		component.setContent("Content");
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content</Doc>"), mTargetStream.str());
	}

	void testComponentAsPlainContent_withoutContent()
	{
		XmlMockUpComponent component;
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc/>"), mTargetStream.str());
	}

	void testComponentAsElement_withoutContent()
	{
		XmlMockUpComponent component("Element",true);
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element/></Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_withContent()
	{
		XmlMockUpComponent component("Element",true);
		component.setContent("Content");
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element></Doc>"), mTargetStream.str());
	}
	
	void testComponentAsAttribute()
	{
		XmlMockUpComponent component("at",false);
		component.setContent("Content");
		XercesDom2ClamObjects dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc at=\"Content\"/>"), mTargetStream.str());
	}



	void testSibblingsContentsAndAttributes_getOrderedAsInserted()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpBasic oneElement("OneElement",true);
		XmlMockUpBasic otherElement("OtherElement",true);
		XmlMockUpBasic content;
		content.setContent("Content");
		dumper.Store(oneElement);
		dumper.Store(content);
		dumper.Store(otherElement);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><OneElement/>Content<OtherElement/></Doc>"), mTargetStream.str());
	}
	

	void testConsecutiveContents_getSpaceSeparation()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpBasic content1;
		content1.setContent("Content1");
		XmlMockUpBasic content2;
		content2.setContent("Content2");
		dumper.Store(content1);
		dumper.Store(content2);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content1 Content2</Doc>"), mTargetStream.str());
	}

	void testNonConsecutiveContents_dontGetSpaceSeparation()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpBasic content1;
		content1.setContent("Content1");
		XmlMockUpBasic content2;
		XmlMockUpBasic element("Element",true);
		content2.setContent("Content2");
		dumper.Store(content1);
		dumper.Store(element);
		dumper.Store(content2);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content1<Element/>Content2</Doc>"), mTargetStream.str());
	}

	void testSibblingsAttributes_getReordered()
	{
		// Not a requirement, just to check the behabiour
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpBasic attribute1("zFirst");
		attribute1.setContent("Content1");
		XmlMockUpBasic attribute2("aSecond");
		attribute2.setContent("Content2");
		dumper.Store(attribute1);
		dumper.Store(attribute2);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc aSecond=\"Content2\" zFirst=\"Content1\"/>"), mTargetStream.str());
	}

	void testComponentAsElement_containingBasicAsPlainContent()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		element.add(content);
		dumper.Store(element);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element></Doc>"), mTargetStream.str());
	}

	void testElementContents_getPrintedBeforeSiblingContent()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		element.setContent("ElementContent");
		element.add(content);
		dumper.Store(element);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>ElementContent Content</Element></Doc>"), mTargetStream.str());
	}

	void testNodesInsertionAfterComponentElement()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		XmlMockUpBasic afterContent;
		afterContent.setContent("AfterContent");
		element.add(content);
		dumper.Store(element);
		dumper.Store(afterContent);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element>AfterContent</Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_containingBasicAsAttribute()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpComponent element("Element",true);
		XmlMockUpBasic attribute("at");
		attribute.setContent("atContent");
		element.add(attribute);
		dumper.Store(element);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element at=\"atContent\"/></Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_containingBasicAsElement()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpComponent componentElement("Component",true);
		componentElement.setContent("ComponentContent");
		XmlMockUpBasic basicElement("Basic",true);
		basicElement.setContent("BasicContent");
		componentElement.add(basicElement);
		dumper.Store(componentElement);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Component>ComponentContent<Basic>BasicContent</Basic></Component></Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_containingComponentAsElement()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpComponent outsideElement("Outside",true);
		outsideElement.setContent("ComponentContent");
		XmlMockUpComponent insideElement("Inside",true);
		insideElement.setContent("InsideContent");
		outsideElement.add(insideElement);
		dumper.Store(outsideElement);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Outside>ComponentContent<Inside>InsideContent</Inside></Outside></Doc>"), mTargetStream.str());
	}

	void testComponentAsContent_containingBasicAsPlainContent()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicContent;
		basicContent.setContent("BasicContent");
		componentContent.add(basicContent);
		dumper.Store(componentContent);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>ComponentContent BasicContent</Doc>"), mTargetStream.str());
	}

	void testComponentAsContent_containingBasicAsAttribute()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicAttribute("at");
		basicAttribute.setContent("BasicContent");
		componentContent.add(basicAttribute);
		dumper.Store(componentContent);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc at=\"BasicContent\">ComponentContent</Doc>"), mTargetStream.str());
	}

	void testComponentAsContent_containingBasicAsElement()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicElement("Basic",true);
		basicElement.setContent("BasicContent");
		componentContent.add(basicElement);
		dumper.Store(componentContent);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>ComponentContent<Basic>BasicContent</Basic></Doc>"), mTargetStream.str());
	}

	void testComponentAsAttribute_containingAnything_childrenHaveNoEffect()
	{
		XercesDom2ClamObjects dumper("Doc");
		XmlMockUpComponent componentAttribute("componentAttribute");
		componentAttribute.setContent("ComponentContent");
		XmlMockUpBasic basicElement("BasicElement",true);
		basicElement.setContent("BasicElementContent");
		XmlMockUpBasic basicAttribute("BasicAttribute",true);
		basicElement.setContent("BasicAttributeContent");
		XmlMockUpBasic basicContent;
		basicContent.setContent("BasicContentContent");
		componentAttribute.add(basicElement);
		componentAttribute.add(basicAttribute);
		componentAttribute.add(basicContent);
		dumper.Store(componentAttribute);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc componentAttribute=\"ComponentContent\"/>"), mTargetStream.str());
	}
#endif

};




} // namespace Test
} // namespace Cuidado

