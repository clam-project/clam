#ifndef _Pool_hxx_
#define _Pool_hxx_


#include "DescriptionAttributes.hxx"

#include <string>
#include <map>
#include <vector>
#include "Assert.hxx"
#include "DataTypes.hxx"
#include "Component.hxx"
#include "XMLAdapter.hxx"
#include "XMLComponentAdapter.hxx"
#include "XMLArrayAdapter.hxx"


/**
 * @defgroup SemanticalAnalysis Semantical Analysis
 * These are the classes that handle the description extraction.
 */



namespace CLAM
{
	/**
	 * A description scope defines a set of attributes which change
	 * their values along a given dimension.
	 * @ingroup SemanticalAnalysis
	 */
	class DescriptionScope
	{
	public:
		typedef std::map<std::string, unsigned> NamesMap;
		typedef std::vector<AbstractAttribute *> Attributes;
	private:
		NamesMap _nameMap;
		Attributes _attributes;
		std::string _scopeName;
	public:
		DescriptionScope(const std::string & name) : _scopeName(name) {}
		~DescriptionScope()
		{
			Attributes::iterator it = _attributes.begin();
			Attributes::iterator end = _attributes.end();
			for (; it!=end; it++)
				delete *it;
		}

		/** @return the scope name */
		const std::string & GetName() const
		{
			return _scopeName;
		}

		/** 
		 * Adds the scope an attribute named 'name' using the template parameter 
		 * as the attribute type.
		 * @arg AttributeType The type for the attribute
		 * @arg name The name for the new attribute
		 */
		template <typename AttributeType>
		void Add(const std::string & name)
		{
			unsigned attributeIndex = _nameMap.size();
			bool inserted = 
				_nameMap.insert(std::make_pair(name,attributeIndex)).second;
			CLAM_ASSERT(inserted,"DescriptionScope::Add, Attribute already present");
			_attributes.push_back(new Attribute<AttributeType>(name));
		}

		/**
		 * Returns the attribute index that can be used for fast access to the 
		 * attribute in a spec or even . 
		 * @warning The index is not a confident reference after serialization.
		 */
		unsigned GetIndex(const std::string & name) const
		{
			NamesMap::const_iterator it = _nameMap.find(name);
			CLAM_ASSERT(it!=_nameMap.end(),
				"Accessing an unexisting attribute inside a scope");
			return it->second;
		}

		unsigned GetNAttributes() const
		{
			return _nameMap.size();
		}

		void * Allocate(unsigned attribute, unsigned size) const
		{
			return _attributes[attribute]->Allocate(size);
		}
		void Deallocate(unsigned attribute, void * buffer) const
		{
			_attributes[attribute]->Deallocate(buffer);
		}

		template <typename AttributeType>
		void CheckType(unsigned attributeIndex, AttributeType *) const
		{
			_attributes[attributeIndex]->CheckType<AttributeType>();
		}

		const std::string & GetAttributeName(unsigned attributeIndex) const
		{
			CLAM_ASSERT(attributeIndex<_attributes.size(),
				"GetAttributeName: Using a wrong index to look up an attribute name");
			AbstractAttribute * attribute = _attributes[attributeIndex];
			return attribute->GetName();
		}
		void DumpAttributeData(Storage & storage, unsigned attribute, void * data, unsigned size) const
		{
			CLAM_ASSERT(data || !size, "Dumping data from a non instantiated attribute");
			_attributes[attribute]->XmlDumpData(storage, data, size);
		}
	};

	/**
	 * Represents a description schema, that is which scopes
	 * will be used, which attributes they have, which extractors
	 * will compute such attributes and where the extractors
	 * are feeded from.
	 * @ingroup SemanticalAnalysis
	 */
	class DescriptionScheme
	{
	private:
		typedef std::map<std::string, unsigned> ScopeMap;
		typedef std::vector<DescriptionScope *> Scopes;
	private:
		Scopes _scopes;
		ScopeMap _scopeNameMap;
	public:
		DescriptionScheme()
		{
		}

		~DescriptionScheme()
		{
			Scopes::iterator it = _scopes.begin();
			Scopes::iterator end = _scopes.end();
			for (; it!=end; it++)
				delete *it;
		}

		template < typename AttributeSpec >
		void AddAttribute(const std::string &scope, const std::string & name)
		{
			typedef typename AttributeSpec::DataType DataType;
			DescriptionScope & theScope = SearchScopeOrAdd(scope);
			theScope.template Add<DataType>(name);
		}

		DescriptionScope & SearchScopeOrAdd(const std::string scopeName)
		{
			const unsigned nScopes = _scopes.size();
			std::pair<ScopeMap::iterator,bool> result = 
				_scopeNameMap.insert(std::make_pair(scopeName,nScopes));

			if (!result.second) return *_scopes[result.first->second];

			DescriptionScope * theScope = new DescriptionScope(scopeName);
			_scopes.push_back(theScope);
			return *theScope;
		}

		unsigned GetScopeIndex(const std::string & name) const
		{
			ScopeMap::const_iterator it = _scopeNameMap.find(name);
			CLAM_ASSERT(it!=_scopeNameMap.end(), "No scope registered with that name");
			return it->second;
		}

		const DescriptionScope & GetScope(unsigned scopeIndex) const
		{
			CLAM_ASSERT(scopeIndex < _scopes.size(), "Accessing an illegal scope index for the description scheme");
			return *_scopes[scopeIndex];
		}

		const DescriptionScope & GetScope(const std::string & name) const
		{
			unsigned scopeIndex = GetScopeIndex(name);
			return GetScope(scopeIndex);
		}
		unsigned GetNScopes() const 
		{
			return _scopes.size();
		}

		const std::string & GetScopeName(unsigned scopeIndex) const
		{
			const DescriptionScope & scope = GetScope(scopeIndex);
			return scope.GetName();
		}
	};

	/**
	 * A container for the attributes values along the differents
	 * contexts of a single scope.
	 * @ingroup SemanticalAnalysis
	 */
	class ScopePool : public Component
	{
	public:
		typedef std::vector<void*> AttributesData;
	private:
		unsigned _size;
		AttributesData _attributes;
		const DescriptionScope & _spec;
		class AttributePoolAdapter : public Component
		{
		public:
			AttributePoolAdapter(const DescriptionScope & scope, unsigned attribute, void * data, unsigned size)
				: _scope(scope), _attribute(attribute), _data(data), _size(size) { }
			const char * GetClassName() const { return "TODO"; }
			void StoreOn(Storage & storage) const
			{
				_scope.DumpAttributeData(storage,_attribute,_data,_size);
			}
			void LoadFrom(Storage & storage)
			{
			}
		private:
			const DescriptionScope & _scope;
			unsigned _attribute;
			void * _data;
			unsigned _size;
		};

	public:
		ScopePool(const DescriptionScope & spec, unsigned size=0)
			: _size(size), _spec(spec), _attributes(spec.GetNAttributes(),(void*)0)
		{
		}
		~ScopePool()
		{
			AttributesData::iterator it = _attributes.begin();
			AttributesData::iterator end = _attributes.end();
			for (unsigned i=0; it!=end; i++, it++)
			{
				if (!*it) continue;
				_spec.Deallocate(i, *it);
				*it=0;
			}
			_size=0;
		}
		const char * GetClassName() const { return "ScopePool"; }
		void StoreOn(Storage & storage) const
		{
			XMLAdapter<std::string> nameAdapter(_spec.GetName(),"name",false);
			storage.Store(nameAdapter);
			XMLAdapter<unsigned> sizeAdapter(_size,"size",false);
			storage.Store(sizeAdapter);
			for (unsigned attribute=0; attribute<_attributes.size(); attribute++)
			{
				if (_size && !_attributes[attribute]) continue;
				AttributePoolAdapter attributeAdapter(_spec, attribute, _attributes[attribute], _size);
				XMLComponentAdapter adapter(attributeAdapter,"AttributePool",true);
				storage.Store(adapter);
			}

		}
		void LoadFrom(Storage & storage)
		{
		}
	private:
		void Reallocate(unsigned newSize)
		{
			_size = newSize;
			AttributesData::iterator it = _attributes.begin();
			AttributesData::iterator end = _attributes.end();
			for (unsigned i=0; it!=end; i++, it++)
			{
				if (!*it) continue;
				_spec.Deallocate(i, *it);
				*it = newSize ? _spec.Allocate(i,_size) : 0;
			}
		}
	public:
		unsigned GetNAttributes() const
		{
			return _spec.GetNAttributes();
		}
		unsigned GetSize() const
		{
			return _size;
		}
		void SetSize(unsigned newSize)
		{
			Reallocate(newSize);
		}

		template <typename AttributeType>
		const AttributeType * GetReadPool(const std::string & name) const
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			CLAM_ASSERT(_attributes[attribPos],"Getting data from a non instanciated attribute");
			return (const AttributeType*) _attributes[attribPos];
		}

		template <typename AttributeType>
		AttributeType * GetWritePool(const std::string & name)
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			if (!_attributes[attribPos])
				_attributes[attribPos] = _spec.Allocate(attribPos,_size);
			return (AttributeType*) _attributes[attribPos];
		}
	};

	/**
	 * Contains the extracted data for a given description target.
	 * It conforms to a given DescriptionScheme
	 * @ingroup SemanticalAnalysis
	 */
	class DescriptionDataPool : public Component
	{
	public:
		DescriptionDataPool(const DescriptionScheme & scheme)
			: _scheme(scheme), _scopePools(_scheme.GetNScopes(),(ScopePool*)0)
		{
		}
		~DescriptionDataPool()
		{
			ScopePools::iterator it = _scopePools.begin();
			ScopePools::iterator end = _scopePools.end();
			for (; it != end; it++)
				if (*it) delete *it;
		}

		/**
		 * Sets the number of contexts (ie. Notes) for the given Scope (Note), 
		 * so that every attribute registered for the scope will have a value 
		 * for every one of those contexts.
		 *
		 * Scopes are like Classes, Contexts are like Class Instances.
		 */
		void SetNumberOfContexts(const std::string & scopeName, unsigned size)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			const DescriptionScope & scope = _scheme.GetScope(scopeIndex);
			_scopePools[scopeIndex] = new ScopePool(scope, size);
		}

		unsigned GetNumberOfContexts(const std::string & scopeName)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			CLAM_ASSERT(_scopePools[scopeIndex],"Getting the Scope size but it is not populated");
			return _scopePools[scopeIndex]->GetSize();
		}

		
		void InstantiateAttribute(const std::string & scopeName, const std::string & attributeName)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			const DescriptionScope & scope = _scheme.GetScope(scopeIndex);
			unsigned attributeIndex = scope.GetIndex(attributeName);
			CLAM_ASSERT(_scopePools[scopeIndex], "Instantianting an attribute inside an unpopulated scope");
		}
		template <typename AttributeType>
		AttributeType * GetAttributePool(const std::string & scopeName, const std::string & attributeName)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			const DescriptionScope & scope = _scheme.GetScope(scopeIndex);

			CLAM_ASSERT(_scopePools[scopeIndex],"Accessing attribute data inside an unpopulated scope");

			return _scopePools[scopeIndex]->template GetWritePool<AttributeType>(attributeName);
		}
		template <typename AttributeType>
		const AttributeType * GetReadAttributePool(const std::string & scopeName, const std::string & attributeName) const
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			const DescriptionScope & scope = _scheme.GetScope(scopeIndex);

			CLAM_ASSERT(_scopePools[scopeIndex],"Accessing attribute data inside an unpopulated scope");

			return _scopePools[scopeIndex]->template GetReadPool<AttributeType>(attributeName);
		}
	// Component Interface

		const char * GetClassName() const { return "DescriptionDataPool"; }
		void StoreOn(Storage & storage) const
		{
			for (unsigned i = 0; i<_scopePools.size(); i++)
			{
				XMLComponentAdapter adapter(*(_scopePools[i]), "ScopePool", true);
				storage.Store(adapter);
			}
		}
		void LoadFrom(Storage & storage)
		{
		}


		/*
		unsigned GetScopeSize(const std::string & scopeName)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			const DescriptionScope & scope = _scheme.GetScope(scopeIndex);
			return scope.GetSize();
		}
		*/
	private:
		const DescriptionScheme & _scheme;
		typedef std::vector<ScopePool*> ScopePools;
		ScopePools _scopePools;
	};

}


#endif// _Pool_hxx_

