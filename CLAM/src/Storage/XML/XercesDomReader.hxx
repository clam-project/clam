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

#include "XercesEncodings.hxx"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <string>
#include <list>


namespace xercesc = XERCES_CPP_NAMESPACE;

namespace CLAM
{

class XercesDomReader : private xercesc::HandlerBase
{
	xercesc::XercesDOMParser * parser;
	public:
		XercesDomReader()
		{
			parser = new xercesc::XercesDOMParser();
		}
		~XercesDomReader()
		{
			delete parser;
		}
		xercesc::DOMDocument * read(std::istream & target)
		{
			std::ostringstream stream;
			char c;
			while (target.get(c)) stream.put(c);
			const char * documentText = stream.str().c_str();
			unsigned length = stream.str().length();
			xercesc::MemBufInputSource* memBufIS = new xercesc::MemBufInputSource
			(
				(const XMLByte*)documentText
				, length
				, "CLAMParser"
				, false
			);


			parser->setErrorHandler(this);
			parser->setValidationScheme(xercesc::XercesDOMParser::Val_Auto);
			parser->setValidationSchemaFullChecking(true);
			parser->setDoNamespaces(true);
			parser->setDoSchema(true);
			parser->setCreateEntityReferenceNodes(false);

			parser->parse(*memBufIS);

			if (parser->getErrorCount())
				throw Err(
					(std::string("\nXML Parser Errors:\n")+
					 RecopilaErrors()).c_str());
			xercesc::DOMDocument *doc = parser->getDocument();
			CLAM_ASSERT(doc,"No errors but document not loaded!");
			return doc;
		}
	private:
		typedef std::list<std::string> Missatges;
		Missatges _errors;
		void error(const xercesc::SAXParseException& e)
		{
			report("Error", e);
		}

		void fatalError(const xercesc::SAXParseException& e)
		{
			report("Fatal Error", e);
		}

		void warning(const xercesc::SAXParseException& e)
		{
			report("Warning", e);
		}

		void report(const std::string & level, const xercesc::SAXParseException& e)
		{
			std::ostringstream stream;
			stream << level << " at file " << L(e.getSystemId())
				<< ", line " << e.getLineNumber()
				<< ", col " << e.getColumnNumber()
				<< ":\n" << L(e.getMessage());
			_errors.push_back(stream.str());
		}
	public:
		std::string RecopilaErrors()
		{
			std::string result;
			Missatges::iterator it = _errors.begin();
			for (; it!=_errors.end(); it++)
				result += *it + "\n";
			return result;
		}
};



}
