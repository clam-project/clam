/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/DOMParser.hpp>

#include "XercesDomPrinter.hxx"
#include <string>

//#define CLAM_INDENT_XML
#define TRACEDUMP if (1); else std::cout

class DOMPrintFormatTarget : public XMLFormatTarget
{
public:
	DOMPrintFormatTarget()  {};
	DOMPrintFormatTarget(std::ostream& target)  {mpTarget=&target;};//added by XA
	~DOMPrintFormatTarget() {};

	// -----------------------------------------------------------------------
	//  Implementations of the format target interface
	// -----------------------------------------------------------------------

	void writeChars(const   XMLByte* const  toWrite,
	                const   unsigned int    count,
                        XMLFormatter * const formatter)
	{
		// Surprisingly, Solaris was the only platform on which
		// required the char* cast to print out the string correctly.
		// Without the cast, it was printing the pointer value in hex.
		// Quite annoying, considering every other platform printed
		// the string with the explicit cast to char* below.
		mpTarget->write((char *) toWrite, count);
	};

private:
	// -----------------------------------------------------------------------
	//  Unimplemented methods.
	// -----------------------------------------------------------------------
	DOMPrintFormatTarget(const DOMPrintFormatTarget& other);
	void operator=(const DOMPrintFormatTarget& rhs);
	std::ostream* mpTarget;//mpTarget added by XA in order to use other ostream than cout
};


XMLFormatter& operator<< (XMLFormatter& strm, const DOMString& s);
std::ostream& PrintDoc(std::ostream& target, DOM_Node& toWrite);

void Inspect (std::ostream& target, DOM_Node& toWrite);


// ---------------------------------------------------------------------------
//  Local data
//
//  gEncodingName
//      The encoding we are to output in. If not set on the command line,
//      then it is defaults to the encoding of the input XML file.
//
// ---------------------------------------------------------------------------
static XMLCh*                   gEncodingName          = 0;
static XMLFormatter::UnRepFlags gUnRepFlags            = XMLFormatter::UnRep_CharRef;
static XMLFormatter*            gFormatter             = 0;


// ---------------------------------------------------------------------------
//  Local const data
//
//  Note: This is the 'safe' way to do these strings. If you compiler supports
//        L"" style strings, and portability is not a concern, you can use
//        those types constants directly.
// ---------------------------------------------------------------------------
static const XMLCh  gEndElement[] = 
// "</"
{ 
	chOpenAngle, chForwardSlash, chNull 
};
static const XMLCh  gEndPI[] = 
// "?>"
{
	chQuestion, chCloseAngle, chNull
};
static const XMLCh  gStartPI[] = 
// "<?"
{
	chOpenAngle, chQuestion, chNull 
};
static const XMLCh  gXMLDecl1[] =
// "<?xml version=\""
{
        chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l
    ,   chSpace, chLatin_v, chLatin_e, chLatin_r, chLatin_s, chLatin_i
    ,   chLatin_o, chLatin_n, chEqual, chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl2[] =
// "\" encoding=\""
{
        chDoubleQuote, chSpace, chLatin_e, chLatin_n, chLatin_c
    ,   chLatin_o, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chEqual
    ,   chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl3[] =
// "\" standalone=\""
{
        chDoubleQuote, chSpace, chLatin_s, chLatin_t, chLatin_a
    ,   chLatin_n, chLatin_d, chLatin_a, chLatin_l, chLatin_o
    ,   chLatin_n, chLatin_e, chEqual, chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl4[] =
// "\"?>\r\n"
{
        chDoubleQuote, chQuestion, chCloseAngle
    ,   chCR, chLF, chNull
};

static const XMLCh  gStartCDATA[] =
// "<![CDATA["
{ 
        chOpenAngle, chBang, chOpenSquare, chLatin_C, chLatin_D,
        chLatin_A, chLatin_T, chLatin_A, chOpenSquare, chNull
};

static const XMLCh  gEndCDATA[] =
// "]]>"
{
    chCloseSquare, chCloseSquare, chCloseAngle, chNull
};
static const XMLCh  gStartComment[] =
// "<!--"
{ 
    chOpenAngle, chBang, chDash, chDash, chNull
};

static const XMLCh  gEndComment[] =
// "-->"
{
    chDash, chDash, chCloseAngle, chNull
};

static const XMLCh  gStartDoctype[] =
// "<!DOCTYPE "
{ 
    chOpenAngle, chBang, chLatin_D, chLatin_O, chLatin_C, chLatin_T,
    chLatin_Y, chLatin_P, chLatin_E, chSpace, chNull
};
static const XMLCh  gPublic[] =
// "PUBLIC \""
{ 
    chLatin_P, chLatin_U, chLatin_B, chLatin_L, chLatin_I,
    chLatin_C, chSpace, chDoubleQuote, chNull
};
static const XMLCh  gSystem[] =
// "SYSTEM \""
{ 
    chLatin_S, chLatin_Y, chLatin_S, chLatin_T, chLatin_E,
    chLatin_M, chSpace, chDoubleQuote, chNull
};
static const XMLCh  gStartEntity[] =
// "<!ENTITY "
{ 
    chOpenAngle, chBang, chLatin_E, chLatin_N, chLatin_T, chLatin_I,
    chLatin_T, chLatin_Y, chSpace, chNull
};
static const XMLCh  gNotation[] =
// "NDATA \""
{ 
    chLatin_N, chLatin_D, chLatin_A, chLatin_T, chLatin_A,
    chSpace, chDoubleQuote, chNull
};


// ---------------------------------------------------------------------------
//  ostream << DOM_Node   
//
//  Stream out a DOM node, and, recursively, all of its children. This
//  function is the heart of writing a DOM tree out as XML source. Give it
//  a document node and it will do the whole thing.
// ---------------------------------------------------------------------------


CLAM::XercesDomPrinter::XercesDomPrinter() 
{
}
CLAM::XercesDomPrinter::~XercesDomPrinter()
{
}
void CLAM::XercesDomPrinter::Print(ostream & os, DOM_Node & toWrite)
{
		PrintDoc(os,toWrite);
}

unsigned tabPosition = 0;
bool gLastWasContent = true;


std::ostream& PrintDoc(std::ostream& target, DOM_Node& toWrite)
{
  
	tabPosition = 0;
	gLastWasContent = true;
	DOMString encNameStr("UTF-8");
	DOM_Node aNode = toWrite;
	if (aNode.getNodeType() == DOM_Node::XML_DECL_NODE)
	{
		DOMString aStr = ((DOM_XMLDecl &)aNode).getEncoding();
		if (aStr != "")
			encNameStr = aStr;
	}
	unsigned int lent = encNameStr.length();
	gEncodingName = new XMLCh[lent + 1];
	XMLString::copyNString(gEncodingName, encNameStr.rawBuffer(), lent);
	gEncodingName[lent] = 0;
	DOMPrintFormatTarget* formatTarget = new DOMPrintFormatTarget(target);    
	try
	{
		gFormatter = new XMLFormatter(gEncodingName, formatTarget, 
			XMLFormatter::NoEscapes, gUnRepFlags);
		target << toWrite << std::endl;
	}
	catch (XMLException& e)
	{
		std::cerr 
			<< "An error occurred during creation of output transcoder. Msg is:"
			<< std::endl
			<< DOMString(e.getMessage()) << std::endl;
	}
	delete formatTarget;
	delete gFormatter;
	delete gEncodingName;

	return target;
}

static const XMLCh endLine[] = { chCR, chLF, chNull };

#ifdef CLAM_INDENT_XML
static const bool gIndentXml = true;
#else
static const bool gIndentXml = false;
#endif

std::ostream& operator<<(std::ostream& target, DOM_Node& toWrite)
{

	// Get the name and value out for convenience
	DOMString   nodeName = toWrite.getNodeName();
	DOMString   nodeValue = toWrite.getNodeValue();
	std::string currentIndentation(tabPosition,'\t');
	unsigned long lent = nodeValue.length();
	bool thisWasContent=false;

	switch (toWrite.getNodeType())
	{
		case DOM_Node::TEXT_NODE:
		{
			if (gIndentXml && !gLastWasContent)
				*gFormatter
					<< endLine << currentIndentation.c_str();
			gFormatter->formatBuf(nodeValue.rawBuffer(), 
				lent, XMLFormatter::CharEscapes);
			thisWasContent=true;
			break;
		}


		case DOM_Node::PROCESSING_INSTRUCTION_NODE :
		{
			*gFormatter << XMLFormatter::NoEscapes << gStartPI  << nodeName;
			if (lent > 0)
			{
				*gFormatter << chSpace << nodeValue;
			}
			*gFormatter << XMLFormatter::NoEscapes << gEndPI;
			break;
		}


		case DOM_Node::DOCUMENT_NODE :
		{

			DOM_Node child = toWrite.getFirstChild();
			while( child != 0)
			{
				target << child << std::endl;
				child = child.getNextSibling();
			}
			break;
		}


		case DOM_Node::ELEMENT_NODE :
		{
		TRACEDUMP << std::string(tabPosition++,'\t') << "Element: " << nodeName << std::endl;
			// The name has to be representable without any escapes
			*gFormatter  << XMLFormatter::NoEscapes
						<< chOpenAngle << nodeName;

			// Output the element start tag.

			// Output any attributes on this element
			DOM_NamedNodeMap attributes = toWrite.getAttributes();
			int attrCount = attributes.getLength();
			for (int i = 0; i < attrCount; i++)
			{
				DOM_Node  attribute = attributes.item(i);

				//
				//  Again the name has to be completely representable. But the
				//  attribute can have refs and requires the attribute style
				//  escaping.
				//
				*gFormatter << XMLFormatter::NoEscapes
				            << chSpace << attribute.getNodeName()
				            << chEqual << chDoubleQuote
				            << XMLFormatter::AttrEscapes
				            << attribute.getNodeValue()
				            << XMLFormatter::NoEscapes
				            << chDoubleQuote;
			}

			//
			//  Test for the presence of children, which includes both
			//  text content and nested elements.
			//
			DOM_Node child = toWrite.getFirstChild();
			if (child != 0)
			{
				// There are children. Close start-tag, and output children.
				// No escapes are legal here
				*gFormatter << XMLFormatter::NoEscapes << chCloseAngle;

				tabPosition++;
				while( child != 0)
				{
					target << child;
					child = child.getNextSibling();
				}

				//
				// Done with children.  Output the end tag.
				//
				tabPosition--;
				if (gIndentXml)
					*gFormatter
						<< endLine << currentIndentation.c_str();
				*gFormatter
					<< XMLFormatter::NoEscapes
					<< gEndElement
					<< nodeName 
					<< chCloseAngle;
			}
			else
			{
				//
				//  There were no children. Output the short form close of
				//  the element start tag, making it an empty-element tag.
				//
				*gFormatter 
					<< XMLFormatter::NoEscapes 
					<< chForwardSlash 
					<< chCloseAngle;
			}
			TRACEDUMP << currentIndentation << "tnemelE: " << nodeName << std::endl;
			break;
		}


		case DOM_Node::ENTITY_REFERENCE_NODE:
		{
			DOM_Node child;
#if 0
			for (child = toWrite.getFirstChild();
			child != 0;
			child = child.getNextSibling())
			{
				target << child;
			}
#else
			//
			// Instead of printing the refernece tree 
			// we'd output the actual text as it appeared in the xml file.
			// This would be the case when -e option was chosen
			//
			*gFormatter << XMLFormatter::NoEscapes << chAmpersand
				<< nodeName << chSemiColon;
#endif
			break;
		}


		case DOM_Node::CDATA_SECTION_NODE:
		{
			*gFormatter << XMLFormatter::NoEscapes << gStartCDATA
			            << nodeValue << gEndCDATA;
			break;
		}


		case DOM_Node::COMMENT_NODE:
		{
			*gFormatter << XMLFormatter::NoEscapes << gStartComment
			            << nodeValue << gEndComment;
			break;
		}


		case DOM_Node::DOCUMENT_TYPE_NODE:
		{
			TRACEDUMP << "Document: " << nodeName << std::endl;
			DOM_DocumentType doctype = (DOM_DocumentType &)toWrite;;

			*gFormatter << XMLFormatter::NoEscapes  << gStartDoctype
			            << nodeName;

			DOMString id = doctype.getPublicId();
			if (id != 0)
			{
				*gFormatter << XMLFormatter::NoEscapes << chSpace << gPublic
					<< id << chDoubleQuote;
				id = doctype.getSystemId();
				if (id != 0)
				{
					*gFormatter << XMLFormatter::NoEscapes << chSpace 
					<< chDoubleQuote << id << chDoubleQuote;
				}
			}
			else
			{
				id = doctype.getSystemId();
				if (id != 0)
				{
					*gFormatter << XMLFormatter::NoEscapes << chSpace << gSystem
						<< id << chDoubleQuote;
				}
			}

			id = doctype.getInternalSubset(); 
			if (id !=0)
				*gFormatter << XMLFormatter::NoEscapes << chOpenSquare
				            << id << chCloseSquare;

			*gFormatter << XMLFormatter::NoEscapes << chCloseAngle;
			TRACEDUMP << "tnemucoD: " << nodeName << std::endl;
			break;
		}


		case DOM_Node::ENTITY_NODE:
		{
			*gFormatter << XMLFormatter::NoEscapes << gStartEntity
			            << nodeName;

			DOMString id = ((DOM_Entity &)toWrite).getPublicId();
			if (id != 0)
				*gFormatter << XMLFormatter::NoEscapes << gPublic
				            << id << chDoubleQuote;

			id = ((DOM_Entity &)toWrite).getSystemId();
			if (id != 0)
				*gFormatter << XMLFormatter::NoEscapes << gSystem
				            << id << chDoubleQuote;

			id = ((DOM_Entity &)toWrite).getNotationName();
			if (id != 0)
				*gFormatter << XMLFormatter::NoEscapes << gNotation
				            << id << chDoubleQuote;

			*gFormatter << XMLFormatter::NoEscapes << chCloseAngle << chCR << chLF;

			break;
		}


		case DOM_Node::XML_DECL_NODE:
		{
			DOMString  str;

			*gFormatter << gXMLDecl1 << ((DOM_XMLDecl &)toWrite).getVersion();

			*gFormatter << gXMLDecl2 << gEncodingName;

			str = ((DOM_XMLDecl &)toWrite).getStandalone();
			if (str != 0)
				*gFormatter << gXMLDecl3 << str;

			*gFormatter << gXMLDecl4;

			break;
		}


		default:
			std::cerr << "Unrecognized node type = "
				<< (long)toWrite.getNodeType() << std::endl;
	}
	gLastWasContent=thisWasContent;
	return target;
}



// ---------------------------------------------------------------------------
//  ostream << DOMString
//
//  Stream out a DOM string. Doing this requires that we first transcode
//  to char * form in the default code page for the system
// ---------------------------------------------------------------------------
std::ostream& operator<< (std::ostream& target, const XMLCh* s)
{
	char *p = XMLString::transcode(s);
	target << p;
	delete [] p;
	return target;
}

std::ostream& operator<< (std::ostream& target, const DOMString& s)
{
	char *p = s.transcode();
	target << p;
	delete [] p;
	return target;
}

XMLFormatter& operator<< (XMLFormatter& strm, const DOMString& s)
{
	unsigned int lent = s.length();

	if (lent <= 0)
		return strm;

	XMLCh*  buf = new XMLCh[lent + 1];
	XMLString::copyNString(buf, s.rawBuffer(), lent);
	buf[lent] = 0;
	strm << buf;
	delete [] buf;
	return strm;
}


static unsigned int indentation=0;

void Inspect (std::ostream& target, DOM_Node& toWrite)
{  
	for (unsigned int i=indentation++; i--; ) target << ". ";
	// Get the name and value out for convenience
	DOMString   nodeName = toWrite.getNodeName();
	DOMString   nodeValue = toWrite.getNodeValue();
	unsigned long lent = nodeValue.length();

	switch (toWrite.getNodeType()) {
		case DOM_Node::TEXT_NODE:
		{
			target << "TEXT:"<<nodeValue<<std::endl;
			break;
		}

		case DOM_Node::PROCESSING_INSTRUCTION_NODE :
		{
			target << "PROCINSTR"<<std::endl;
			break;
		}


        case DOM_Node::DOCUMENT_NODE :
        {
			target << "ROOT"<<std::endl;
			DOM_Node child = toWrite.getFirstChild();
			while( child != 0)
			{
				Inspect(target,child);
				child = child.getNextSibling();
			}
			break;
        }


		case DOM_Node::ELEMENT_NODE :
		{
			target << "ELEMENT:"<<nodeName<<std::endl;

			DOM_NamedNodeMap attributes = toWrite.getAttributes();
			int attrCount = attributes.getLength();
			for (int i = 0; i < attrCount; i++)
			{
				for (unsigned int i=indentation++; i--; ) target << ". ";
				DOM_Node  attribute = attributes.item(i);
				target << "ATTRIBUTE:"<<attribute.getNodeName()<<"="<<attribute.getNodeValue()<< std::endl;
				indentation--;  
			}

			DOM_Node child = toWrite.getFirstChild();
			if (child != 0)
			{
				while( child != 0)
				{
					Inspect(target, child);
					child = child.getNextSibling();
				}

			}
			break;
		}


		case DOM_Node::ENTITY_REFERENCE_NODE:
		{
			target << "ENTITY"<<std::endl;
			break;
		}


		case DOM_Node::CDATA_SECTION_NODE:
		{
			target << "CDATA"<<std::endl;
			break;
		}


		case DOM_Node::COMMENT_NODE:
		{
			target << "COMMENT"<<std::endl;
			break;
		}


		case DOM_Node::DOCUMENT_TYPE_NODE:
		{
			target << "DOCUMENT TYPE"<<std::endl;
			break;
		}


		case DOM_Node::ENTITY_NODE:
		{
			target << "ENTITYNODE"<<std::endl;
			break;
		}


		case DOM_Node::XML_DECL_NODE:
		{
			target << "DECLNODE"<<std::endl;
			break;
		}


		default:
			std::cerr << "Unrecognized node type = "
				<< (long)toWrite.getNodeType() << std::endl;
	}
	indentation--;
}

