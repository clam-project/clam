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

//////////////////////////////////////////////////////////////////////
// XMLStorage.cxx
// Description: A storage in XML format for CLAM objects
//////////////////////////////////////////////////////////////////////

#ifndef _XMLStorage_
#define _XMLStorage_

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
	bool _useIndentation;
/**
 * @name Atomic primitives to Dump and Restore
 * To be used only when you need to twickle the default behavior for summary operations
 * (Dump, Restore...).
 */
// @{
public:
	XmlStorage();
	~XmlStorage();

public:
	void Read(std::istream & is); ///< Loads the DOM from the stream and selects the root node
	void Create(const std::string name); ///< Creates a root node and selects it
	void WriteSelection(std::ostream & os); ///< Dumps the selected node on the stream
	void WriteDocument(std::ostream & os); ///< Dumps the full document on the stream
	void DumpObject(const Component & component); ///< Holds the component DOM to the selected node
	void RestoreObject(Component & component); ///< Restores the component from the selected node
	/**
	 * @brief Sets the selection at the specified path
	 * @todo Not implemented yet
	 */
	void Select(const std::string & path);
	/**
	 * Changes whether to output pretty formated XML or not.
	 * By default, indentation and new lines are not inserted resulting
	 * in compact XML but dificult for humans to read. Use this method,
	 * to enable the indentation.
	 * @todo Not implemented yet
	 */
	void UseIndentation(bool useIndentation);
/// @}

/**
 * @name Summary operations to Dump and Restore
 * Those operations are the simplest ones to use to dump and restore
 * your objects in XML.
 */
public:

	/**
	 * Dumps a Component as XML onto the named file with name as the root element.
	 */
	static void Dump(const Component & obj, const std::string & rootName, std::ostream & os)
	{
		XmlStorage storage;
		storage.Create(rootName);
		storage.DumpObject(obj);
//		storage.UseIndentation(true);
		storage.WriteSelection(os);
	}

	/**
	 * Restore a Component from the given istream.
	 */
	static void Restore(Component & obj, std::istream & is)
	{
		XmlStorage storage;
		storage.Read(is);
		storage.RestoreObject(obj);
	}

	/**
	 * Restore a Component from the xml fragment on the given xpath of the given document.
	 * @todo Not implemented
	 */
	static void RestorePartialDocument(Component & obj, const std::string & path, std::istream & is)
	{
		XmlStorage storage;
		storage.Read(is);
		storage.Select(path);
		storage.RestoreObject(obj);
	}

	/**
	 * Append the xml fragment corresponding to the given component
	 * on the given xpath of an existing i/o stream.
	 * @todo Not implemented
	 */
	static void AppendToDocument(const Component & obj, const std::string & path, std::iostream & str);

	/**
	 * Dump a Component from the named XML file.
	 */
	static void Dump(const Component & obj, const std::string & rootName, const std::string & filename);

	/**
	 * Restore a Component from the named XML file.
	 */
	static void Restore(Component & obj, const std::string & filename);

/**
 * @name Interface for Components to load/store their subitems
 * This is the interface to be used by the CLAM::Component::StoreOn 
 * and CLAM::Component::LoadFrom implementers.
 */
// @{
public:
	/**
	 * Components should use that function in their LoadFrom in order to store
	 * their subitems wrapped with XML*Adapters.
	 */
	void Store(const Storable & storable);
	/**
	 * Components should use that function in their LoadFrom in order to load
	 * their subitems wrapped with XML*Adapters.
	 */
	bool Load(Storable & storable);
// @}

// Private helper functions
private:
	bool LoadContentAndChildren(XMLable* xmlable);
	void StoreContentAndChildren(const XMLable * xmlable);
	void StoreChildrenIfComponent(const XMLable * xmlable);
	void AddContentToElement(const std::string & content);

};

/** For maintaining compatibility with deprecated class name  */
typedef XmlStorage XMLStorage;

} // namespace CLAM

/**
 * @class CLAM::XmlStorage
 * Provides XML format storage for CLAM Component's.
 * Any class that derives from Component can be dumped and
 * restored in XML (http://www.w3.org/XML) format.
 * Components includes Arrays, DynamicType, ProcessingConfig,
 * ProcessingData...
 *
 * @warning XML storage is a very expensive task when dealing
 * 	with big amounts of numeric data.
 *
 * @section XMLStaticInterface Dumping and restoring a component on a file
 * In its simplest usage, you can use the static functions
 * Dump and Restore.
 * @code
 * MyComponent myComponent;
 *
 * // Here you can modify your Component
 *
 * CLAM::XmlStorage::Dump(myComponent, "Document", "MyComponent.xml")
 * @endcode
 *
 * Later you can restore the component:
 * @code
 * // An unmodified default constructed object!!!
 * MyComponent comp;
 * CLAM::XmlStorage::Restore(comp, "mycomponent.xml");
 * @endcode
 *
 * Dump and Restore are overloaded to accept any C++ stream instead of a filename.
 *
 * Although is not implemented yet, you could use the AppendToDocument
 * static function to add an object in a given path of an existing XML file,
 * or using RestorePartialDocument to restore the object taking an XML fragment.
 * Vote for them in the CLAM stories if you are interested in such functionality
 * to be prioritized.
 * 
 */

/**
 * @defgroup XmlDumpingAndRestoring XML: Dumping and Restoring CLAM Components in XML format
 * 
 */

#endif//_XMLStorage_
