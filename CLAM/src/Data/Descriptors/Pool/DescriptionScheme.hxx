#ifndef _DescriptionScheme_hxx_
#define _DescriptionScheme_hxx_


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
}


#endif// _DescriptionScheme_hxx_

