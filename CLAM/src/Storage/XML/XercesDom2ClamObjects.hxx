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

#ifndef _XERCES_DOM_2_CLAM_OBJECTS_HXX_
#define _XERCES_DOM_2_CLAM_OBJECTS_HXX_

#include "XercesDomDocumentHandler.hxx"
#include "XMLable.hxx"
#include "Assert.hxx"
#include "Component.hxx"


#include <list>
#include <string>

namespace xercesc = XERCES_CPP_NAMESPACE;

namespace CLAM
{

class XercesDom2ClamObjects : public Storage
{
	XercesDomDocumentHandler _document;
	XercesDomReadingContext _rootContext;
	XercesDomReadingContext * _readContext;

public:
	XercesDom2ClamObjects(xercesc::DOMDocument * document)
		: _rootContext(document->getDocumentElement())
	{
		_readContext=&_rootContext;
	}
	~XercesDom2ClamObjects()
	{
	}
public:
#ifdef NEVERDEFINED
	// Solo este codigo del load esta cppunitao
	bool Load(Storable & storable)
	{
		XMLable * xmlable = dynamic_cast<XMLable *>(&storable);
		if (xmlable->IsXMLElement())
		{
			if (!_readContext->findElement(xmlable->XMLName()))
				return false;
		}
		return xmlable->XMLContent(_readContext->reachableContent());
	}
#endif
	void Store(const Storable & storable)
	{
	}
	bool Load(Storable & storable)
	{
		XMLable * xmlable = dynamic_cast<XMLable *>(&storable);
		if (!xmlable) return false;

		if (xmlable->IsXMLText())
			return LoadContentAndChildren(xmlable);

		if (xmlable->IsXMLAttribute())
		{
			std::stringstream stream;
			if (!_readContext->extractAttribute(xmlable->XMLName(), stream))
				return false;
			return xmlable->XMLContent(stream);
		}

		if (xmlable->IsXMLElement())
		{
			if (!_readContext->findElement(xmlable->XMLName())) return false;
			XercesDomReadingContext innerContext(_readContext, xmlable->XMLName());
			_readContext = & innerContext;
			LoadContentAndChildren(xmlable);
			_readContext = innerContext.release();
		//	addErrors(innerContext.errors());
			return true;
		}

		CLAM_ASSERT(false, "A weird XMLable inserted");
	}
	bool LoadContentAndChildren(XMLable* xmlable)
	{
		bool result = xmlable->XMLContent(_readContext->reachableContent());
		Component * component = dynamic_cast<Component*>(xmlable);
		if (component) component->LoadFrom(*this);
		return result;
	}
};


}


#endif//_XERCES_DOM_2_CLAM_OBJECTS_HXX_
