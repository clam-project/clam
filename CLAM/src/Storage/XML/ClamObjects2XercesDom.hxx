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

#ifndef _CLAM_OBJECTS_2_XERCES_DOM_HXX_
#define _CLAM_OBJECTS_2_XERCES_DOM_HXX_

#include "XercesEncodings.hxx"
#include "XMLable.hxx"
#include "Assert.hxx"
#include "Component.hxx"
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>


#include <vector>

namespace xercesc = XERCES_CPP_NAMESPACE;

namespace CLAM
{

class ClamObject2XercesDom : public Storage
{
	xercesc::DOMElement * _currentElement;
	xercesc::DOMDocument * _document;
	bool _lastWasContent;
public:
	ClamObject2XercesDom(const std::string name)
	{
		xercesc::XMLPlatformUtils::Initialize();
		xercesc::DOMImplementation * imp = 
			xercesc::DOMImplementation::getImplementation();
		_document = imp->createDocument(
			0, //X("2003-04.clam05.iua.mtg.upf.es"), // root element namespace URI.
			X(name.c_str()), // root element name
			0  // document type object (DTD).
		);
		_lastWasContent=false;
		_currentElement=_document->getDocumentElement();
	}
	~ClamObject2XercesDom()
	{
		_document->release();
		xercesc::XMLPlatformUtils::Terminate();
	}
	xercesc::DOMDocument * getDom()
	{
		return _document;
	}
	bool Load(Storable & storable)
	{
		return false;
	}
	void Store(const Storable & storable)
	{
		const XMLable & xmlable = dynamic_cast<const XMLable &>(storable);
		const char * name = xmlable.XMLName();
		if (!name)
		{
			AddContentToElement(xmlable.XMLContent());
			StoreSibblingsIfComponent(xmlable);
			return;
		}
		if (xmlable.IsXMLElement())
		{
			_lastWasContent=false;
			xercesc::DOMElement * domElement = _document->createElement(X(name));
			_currentElement->appendChild(domElement);
			xercesc::DOMElement * oldElement = _currentElement;
			_currentElement = domElement;
			AddContentToElement(xmlable.XMLContent());
			StoreSibblingsIfComponent(xmlable);
			_currentElement = oldElement;
			_lastWasContent=false;
			return;
		}
		if (xmlable.IsXMLAttribute())
		{
			_currentElement->setAttribute(X(name),X(xmlable.XMLContent().c_str()));
			return;
		}
		CLAM_ASSERT(false,"Component not used");
	}

	void StoreSibblingsIfComponent(const XMLable & xmlable)
	{
		try { 
			const Component & component = 
				dynamic_cast<const Component &>(xmlable);
			component.StoreOn(*this);
		}
		catch (std::bad_cast &) { }
	}

	void AddContentToElement(const std::string & content)
	{
		if (content=="") return;
		if (_lastWasContent)
		{
			xercesc::DOMText * domContent = _document->createTextNode(X(" "));
			_currentElement->appendChild(domContent);
		}
		xercesc::DOMText * domContent = _document->createTextNode(X(content.c_str()));
		_currentElement->appendChild(domContent);
		_lastWasContent = true;
	}

};

	
}


#endif//_CLAM_OBJECTS_2_XERCES_DOM_HXX_
