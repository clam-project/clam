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
#include "XercesInitializer.hxx"
#include "XercesDomDocumentHandler.hxx"
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

class XercesDomWritingContext
{
	xercesc::DOMElement * _currentElement;
	XercesDomWritingContext * _parent;
public:
	XercesDomWritingContext(xercesc::DOMElement * element)
	{
		_parent = 0;
		_currentElement = element;
	}
	XercesDomWritingContext(XercesDomDocumentHandler & docHandler)
	{
		_parent=0;
		_currentElement = docHandler.getSelection();
	}

	XercesDomWritingContext(XercesDomWritingContext * parent, const char * name)
	{
		_parent = parent;
		xercesc::DOMElement * parentElement = parent->_currentElement;
		_currentElement = parentElement->getOwnerDocument()->createElement(X(name));
		parentElement->appendChild(_currentElement);
	}

	XercesDomWritingContext * release()
	{
		return _parent;
	}
	
	void addAttribute(const char * name, const char * value)
	{
		_currentElement->setAttribute(X(name),X(value));
	}
	void addContent(const char * content)
	{
		xercesc::DOMText * domContent = _currentElement->getOwnerDocument()->createTextNode(X(content));
		_currentElement->appendChild(domContent);
	}
};

class ClamObject2XercesDom : public Storage
{
	XercesDomDocumentHandler _documentHandler;
	XercesDomWritingContext * _writeContext;
	XercesDomWritingContext * _rootContext;
	bool _lastWasContent;
public:
	ClamObject2XercesDom()
	{
		_writeContext = 0;
		_rootContext = 0;
		_lastWasContent = true;
	}
	~ClamObject2XercesDom()
	{
		delete _rootContext;
	}
	void Create(const std::string name)
	{
		_documentHandler.create(name.c_str());
		_lastWasContent=false;
		_rootContext= new XercesDomWritingContext(_documentHandler);
		_writeContext = _rootContext;
	}
	bool Load(Storable & storable)
	{
		return false;
	}
	void Store(const Storable & storable)
	{
		const XMLable * xmlable = dynamic_cast<const XMLable *>(&storable);
		const char * name = xmlable->XMLName();
		if (!name)
		{
			StoreContentAndChildren(xmlable);
			return;
		}
		if (xmlable->IsXMLAttribute())
		{
			_writeContext->addAttribute(name,xmlable->XMLContent().c_str());
			return;
		}
		if (xmlable->IsXMLElement())
		{
			_lastWasContent=false;
			XercesDomWritingContext newContext(_writeContext, name);
			_writeContext = & newContext;
			StoreContentAndChildren(xmlable);
			_writeContext = newContext.release();
			_lastWasContent=false;
			return;
		}
		CLAM_ASSERT(false,"A weird XMLable inserted");
	}
private:

	void StoreContentAndChildren(const XMLable * xmlable)
	{
		AddContentToElement(xmlable->XMLContent());
		StoreChildrenIfComponent(xmlable);
	}

	void StoreChildrenIfComponent(const XMLable * xmlable)
	{
		const Component * component = dynamic_cast<const Component *>(xmlable);
		if (component) component->StoreOn(*this);
	}

	void AddContentToElement(const std::string & content)
	{
		if (content=="") return;
		if (_lastWasContent)
			_writeContext->addContent(" ");
		_writeContext->addContent(content.c_str());
		_lastWasContent = true;
	}

public:
	void WriteSelection(std::ostream & os)
	{
		_documentHandler.writeSelection(os);
	}
};

	
}


#endif//_CLAM_OBJECTS_2_XERCES_DOM_HXX_
