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
	 * A container for the values for a single attribute along the scope.
	 * @ingroup SemanticalAnalysis
	 */
	class AttributePool : public Component
	{
	public:
		AttributePool()
		{
			_data=0;
			_size=0;
			_attribute=0;
		}
		void SetDefinition(const AbstractAttribute & attribute)
		{
			_attribute = & attribute;
		}
		const char * GetClassName() const { return "AttributePool"; }
		void StoreOn(Storage & storage) const
		{
			_attribute->XmlDumpData(storage, _data, _size);
		}
		void LoadFrom(Storage & storage)
		{
			_attribute->XmlRestoreData(storage, _data, _size);
		}
		void * GetData() { return _data; }
		const void * GetData() const { return _data; }
		void Allocate(unsigned size)
		{
			_data = _attribute->Allocate(size);
			_size=size;
		}
		void Deallocate()
		{
			if (!_data) return;
			_attribute->Deallocate(_data);
			_data = 0;
		}
	private:
		void * _data;
		const AbstractAttribute * _attribute;
		unsigned _size;
	};

	/**
	 * A container for the attributes values along the differents
	 * contexts of a single scope.
	 * @ingroup SemanticalAnalysis
	 */
	class ScopePool : public Component
	{
	public:
		typedef std::vector<AttributePool> AttributesData;
	private:
		unsigned _size;
		AttributesData _attributePools;
		const DescriptionScope & _spec;
	public:
		ScopePool(const DescriptionScope & spec, unsigned size=0)
			: _size(size), _spec(spec), _attributePools(spec.GetNAttributes())
		{
			AttributesData::iterator it = _attributePools.begin();
			AttributesData::iterator end = _attributePools.end();
			for (unsigned i=0; it!=end; i++, it++)
			{
				it->SetDefinition(_spec.GetAttribute(i));
			}
		}
		~ScopePool()
		{
			AttributesData::iterator it = _attributePools.begin();
			AttributesData::iterator end = _attributePools.end();
			for (; it!=end; it++) it->Deallocate();
			_size=0;
		}
		const char * GetClassName() const { return "ScopePool"; }
		void StoreOn(Storage & storage) const
		{
			XMLAdapter<std::string> nameAdapter(_spec.GetName(),"name",false);
			storage.Store(nameAdapter);
			XMLAdapter<unsigned> sizeAdapter(_size,"size",false);
			storage.Store(sizeAdapter);
			for (unsigned attribute=0; attribute<_attributePools.size(); attribute++)
			{
				if (_size && !_attributePools[attribute].GetData()) continue;
				XMLComponentAdapter adapter(_attributePools[attribute],"AttributePool",true);
				storage.Store(adapter);
			}
		}
		void LoadFrom(Storage & storage)
		{
			std::string name;
			XMLAdapter<std::string> nameAdapter(name,"name",false);
			storage.Load(nameAdapter);
			CLAM_ASSERT(name==_spec.GetName(),"Loading an scope pool for a different attribute");

			unsigned newSize;
			XMLAdapter<unsigned> sizeAdapter(newSize,"size",false);
			storage.Load(sizeAdapter);
			Reallocate(newSize);
			for (unsigned attribute=0; attribute<_attributePools.size(); attribute++)
			{
				_attributePools[attribute].Deallocate();
				_attributePools[attribute].Allocate(_size);
//				if (_size && !_attributePools[attribute].GetData()) continue;
				XMLComponentAdapter adapter(_attributePools[attribute],"AttributePool",true);
				storage.Load(adapter);
			}
		}
	private:
		void Reallocate(unsigned newSize)
		{
			_size = newSize;
			AttributesData::iterator it = _attributePools.begin();
			AttributesData::iterator end = _attributePools.end();
			for (unsigned i=0; it!=end; i++, it++)
			{
				if (!it->GetData()) continue;
				it->Deallocate();
				if (newSize) it->Allocate(_size);
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
			CLAM_ASSERT(_attributePools[attribPos].GetData(),"Getting data from a non instanciated attribute");
			return (const AttributeType*) _attributePools[attribPos].GetData();
		}

		template <typename AttributeType>
		AttributeType * GetWritePool(const std::string & name)
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			if (!_attributePools[attribPos].GetData())
				_attributePools[attribPos].Allocate(_size);
			return (AttributeType*) _attributePools[attribPos].GetData();
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
			for (unsigned i = 0; i<_scopePools.size(); i++)
			{
				const DescriptionScope & scope = _scheme.GetScope(i);
				_scopePools[i] = new ScopePool(scope,0);
				XMLComponentAdapter adapter(*(_scopePools[i]), "ScopePool", true);
				storage.Load(adapter);
			}
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

