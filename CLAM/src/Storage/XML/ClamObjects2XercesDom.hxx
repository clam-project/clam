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

#include "Storage.hxx"
#include <iosfwd>
#include <string>


namespace CLAM
{

class XMLable;
class Component;
class XercesDomDocumentHandler;
class XercesDomWritingContext;
class XercesDomReadingContext;

class XmlStorage : public Storage
{
	XercesDomDocumentHandler * _documentHandler;
	XercesDomWritingContext * _writeContext;
	XercesDomReadingContext * _readContext;
	bool _lastWasContent;
public:
	XmlStorage();
	~XmlStorage();

// Final user interface (Atomic operations)
public:
	void Read(std::istream & is);
	void Create(const std::string name);
	void WriteSelection(std::ostream & os);
	void WriteDocument(std::ostream & os);
	void DumpObject(const Component & component);
	void RestoreObject(Component & component);
	void Select(const std::string & path);
	void UseIndentation(bool useIndentation);
// Final User static interface (Summary operations)
public:

	static void Dump(const Component & obj, const std::string & rootName, std::ostream & os)
	{
		XmlStorage storage;
		storage.Create(rootName);
		storage.DumpObject(obj);
		storage.WriteSelection(os);
	}

	static void Restore(Component & obj, std::istream & is)
	{
		XmlStorage storage;
		storage.Read(is);
		storage.RestoreObject(obj);
	}

	static void Restore(Component & obj, const std::string & filename);

	static void RestorePartialDocument(Component & obj, const std::string & path, std::istream & is)
	{
		XmlStorage storage;
		storage.Read(is);
		storage.Select(path);
		storage.RestoreObject(obj);
	}

	static void AppendToDocument(Component & obj, const std::string & path, std::iostream & str);

// Interface for Components to load/store their subitems
public:
	void Store(const Storable & storable);
	bool Load(Storable & storable);

// Private helper functions
private:
	bool LoadContentAndChildren(XMLable* xmlable);
	void StoreContentAndChildren(const XMLable * xmlable);
	void StoreChildrenIfComponent(const XMLable * xmlable);
	void AddContentToElement(const std::string & content);

};

/** For maintaining compatibility with deprecated class name  */
typedef XmlStorage XMLStorage;


}


#endif//_CLAM_OBJECTS_2_XERCES_DOM_HXX_
