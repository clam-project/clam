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
#include "DescriptionScheme.hxx"


/**
 * @defgroup SemanticalAnalysis Semantical Analysis
 * These are the classes that handle the description extraction.
 */



namespace CLAM
{
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

		unsigned GetNumberOfContexts(const std::string & scopeName) const
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

