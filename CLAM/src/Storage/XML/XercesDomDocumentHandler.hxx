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
#ifndef _XercesDomDocumentHandler_hxx_
#define _XercesDomDocumentHandler_hxx_
#include "XercesDomReader.hxx"
#include "XercesDomWriter.hxx"
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include "XercesEncodings.hxx"

namespace CLAM 
{

class XercesDomDocumentHandler
{
	xercesc::DOMDocument * _document;
	xercesc::DOMElement * _selection;
	xercesc::XercesDOMParser * _parser;
	void releaseIfAnyDocument()
	{
		if (!_document) return;
		if (_parser)
		{
			delete _parser;
			_parser=0;
		}
		else 
		{
			_document->release();
		}
		_document=0;
	}
public:
	XercesDomDocumentHandler()
	{
		_document = 0;
		_selection = 0;
		_parser = 0;
	}
	~XercesDomDocumentHandler()
	{
		releaseIfAnyDocument();
	}
	void setDocument(xercesc::DOMDocument * document)
	{
		_document = document;
		_selection = _document->getDocumentElement();
	}
	void selectPath(const char * path){}
	xercesc::DOMElement * getSelection()
	{
		return _selection;
	}
	void create(const char * rootName)
	{
		releaseIfAnyDocument();
		xercesc::DOMImplementation * imp =
			xercesc::DOMImplementation::getImplementation();
		xercesc::DOMDocument * domDoc = imp->createDocument(0,X(rootName),0);
		setDocument(domDoc);
	}
	void read(std::istream & stream)
	{
		XercesDomReader reader;
		xercesc::DOMDocument * domDoc;
		domDoc = reader.read(stream);
		setDocument(domDoc);
		_parser = reader.adoptParser();
	}
	void writeDocument(std::ostream & os)
	{
		XercesDomWriter writer;
		writer.write(os,_document);

	}
	void writeSelection(std::ostream & os)
	{
		XercesDomWriter writer;
		writer.write(os,_selection);

	}
};

}


#endif//_XercesDomDocumentHandler_hxx_

