#ifndef _DescriptionScheme_hxx_
#define _DescriptionScheme_hxx_


#include "DescriptionScope.hxx"

/**
 * @defgroup SemanticalAnalysis Semantical Analysis
 * These are the classes that handle the description extraction.
 */



namespace CLAM
{
	/**
	 * Represents a description schema for feature extraction.
	 * The description scheme consists on: 
	 * - a set of scopes (DescriptionScope) each scope defines a
	 *   kind of target for the attributes.
	 *   (note scope, sample scope, frame scope, phrase scope, sample scope...)
	 * - a set of attributes (Attribute) for each scope and its type.
	 *
	 * You can use a DescriptionScheme in order to instantiate
	 * a DescriptionDataPool, that will contain the real data for
	 * a given concrete sound sound.
	 * DescriptionSchema is the definition and the DescriptionDataPool
	 * is the instance for a given case.
	 *
	 * @todo The DescriptionSchema should also contain
	 * which Extractors are used to compute the attributes and
	 * where the Extractors takes the data from.
	 *
	 * @see DescriptionDataPool
	 * 
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

