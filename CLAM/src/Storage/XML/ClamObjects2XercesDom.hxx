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

#include "XercesDomDocumentHandler.hxx"
#include "XMLable.hxx"
#include "Assert.hxx"
#include "Component.hxx"


#include <vector>

namespace xercesc = XERCES_CPP_NAMESPACE;

namespace CLAM
{

class ClamObject2XercesDom : public Storage
{
	XercesDomDocumentHandler _documentHandler;
	XercesDomWritingContext * _writeContext;
	XercesDomReadingContext * _readContext;
	bool _lastWasContent;
public:
	ClamObject2XercesDom()
	{
		_writeContext = 0;
		_lastWasContent = true;
		_readContext = 0;
	}
	~ClamObject2XercesDom()
	{
	}

// Final user interface
public:
	void Read(std::istream & is)
	{
		_documentHandler.read(is);
	}

	void Create(const std::string name)
	{
		_documentHandler.create(name.c_str());
		_lastWasContent=false;
	}

	void WriteSelection(std::ostream & os)
	{
		_documentHandler.writeSelection(os);
	}

	void WriteDocument(std::ostream & os)
	{
		_documentHandler.writeDocument(os);
	}

	void DumpObject(const Component & component)
	{
		XercesDomWritingContext rootContext(_documentHandler);
		_writeContext = & rootContext;
		component.StoreOn(*this);
	}

	void RestoreObject(Component & component)
	{
		XercesDomReadingContext rootContext(_documentHandler);
		_readContext = & rootContext;
		component.LoadFrom(*this);
	}

	void Select(const std::string & path)
	{
		CLAM_ASSERT(false,"XMLStorage::Select not implemented yet");
	}

// Final User static interface
public:

	static void Dump(std::ostream & os, const Component & obj, const std::string & rootName)
	{
		ClamObject2XercesDom storage;
		storage.Create(rootName);
		storage.DumpObject(obj);
		storage.WriteDocument(os);
	}

	static void Restore(std::istream & is, Component & obj)
	{
		ClamObject2XercesDom storage;
		storage.Read(is);
		storage.RestoreObject(obj);
	}

	static void RestorePartialDocument(std::istream & is, Component & obj, const std::string & path)
	{
		ClamObject2XercesDom storage;
		storage.Read(is);
		storage.Select(path);
		storage.RestoreObject(obj);
	}

	static void AppendToDocument(std::iostream & str, Component & obj, const std::string & path)
	{
		ClamObject2XercesDom storage;
		storage.Read(str);
		storage.Select(path);
		storage.DumpObject(obj);
		storage.WriteDocument(str);
	}

	

// Interface for Components to load/store their subitems
public:
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

	bool Load(Storable & storable)
	{
		XMLable * xmlable = dynamic_cast<XMLable *>(&storable);
		if (!xmlable) return false;

		if (xmlable->IsXMLText())
			return LoadContentAndChildren(xmlable);

		if (xmlable->IsXMLElement())
		{
			if (!_readContext->findElement(xmlable->XMLName()))
				return false;
			XercesDomReadingContext innerContext(_readContext, xmlable->XMLName());
			_readContext = &innerContext;
			LoadContentAndChildren(xmlable);
			_readContext = innerContext.release();
		//	addErrors(innerContext.errors());
			return true;
		}

		// TODO: Test Attributes
		if (xmlable->IsXMLAttribute())
		{
			std::stringstream stream;
			if (!_readContext->extractAttribute(xmlable->XMLName(), stream))
				return false;
			return xmlable->XMLContent(stream);
		}

		CLAM_ASSERT(false, "A weird XMLable inserted");
	}
private:
	bool LoadContentAndChildren(XMLable* xmlable)
	{
		bool result = xmlable->XMLContent(_readContext->reachableContent());
		Component * component = dynamic_cast<Component*>(xmlable);
		if (component) component->LoadFrom(*this);
		return result;
	}

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

};

//typedef ClamObject2XercesDom XercesDom2ClamObjects;
	
}


#endif//_CLAM_OBJECTS_2_XERCES_DOM_HXX_
