#ifndef _Pool_hxx_
#define _Pool_hxx_

#include "ScopePool.hxx"
#include "DescriptionScheme.hxx"

namespace CLAM
{
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

		/** @todo Should be deprecated?? */
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

