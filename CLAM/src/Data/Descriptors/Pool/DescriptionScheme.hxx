#ifndef _DescriptionScheme_hxx_
#define _DescriptionScheme_hxx_


#include "DescriptionScope.hxx"


/**
 * @defgroup SemanticalAnalysis Semantical Analysis
 * This module explains how to use CLAM to handle audio description
 * extraction using the DescriptionScheme object and its relatives.
 * They will allow you to do the extraction in a modular and incremental way
 * taking from other projects the parts you are interested in and
 * adding your own descriptors in a incremental way.
 *
 * This module intends to implement the system described on
 * http://www.iua.upf.es/mtg/clam/devel/doc/descriptors/Descriptors.html
 * but there is still some way to achieve the full functionality
 * described in there.
 *
 * @section DescriptionSpecification Defining and instanciating descriptors
 *
 * The central object for description extraction is the DescriptionScheme.
 * The description scheme (CLAM::DescriptionScheme) defines 
 * which are the attributes (CLAM::Attribute) we want to compute.
 * You can relate attributes to a name and a type and you
 * can organize attributes in different ''scopes''.
 * You can understand a scope (CLAM::DescriptionScope) as 
 * the kind of target for a given set of attributes.
 * For example, we normaly talk about
 * note scope, sample scope, frame scope, phrase scope...
 * that means that a given attribute will have a value for
 * every single note, sample, frame, phrase...
 *
 * @code
 * CLAM::DescriptionScheme scheme;
 * scheme.AddAttribute <CLAM::TData>    ("AudioSample", "SignalLevel");
 * scheme.AddAttribute <CLAM::TData>    ("AudioSample", "FilteredSignal");
 * scheme.AddAttribute <SamplePosition> ("Frame", "Center");
 * scheme.AddAttribute <CLAM::TData>    ("Frame", "Energy");
 * scheme.AddAttribute <CLAM::TData>    ("Frame", "RMS");
 * scheme.AddAttribute <CLAM::Spectrum> ("Frame", "SpectralDistribution");
 * scheme.AddAttribute <FramePosition>  ("Note", "Onset");
 * scheme.AddAttribute <CLAM::Pitch>    ("Note", "Pitch");
 * @endcode
 * 
 * The description scheme only specifies the attribute organization.
 * The real values are hold into the data pool (CLAM::DescriptionDataPool).
 * An instance of a DescriptionDataPool will hold the attributes
 * extracted from a single description source (ie, an audio).
 * It take the structure defined by the description scheme.
 *
 * @code
 * CLAM::DescriptionDataPool pool(scheme);
 * @endcode
 *
 * See the documentation for CLAM::DescriptionDataPool to see how to work
 * with it directly.
 *
 * So, summarizing:
 * - A description scheme defines attributes to be computed
 * - Every attribute has its name and type and is related to a given scope
 * - A scope specifies the kind of attribute target
 * - A description data pool is the real container for the values computed by extraction.
 * - A description data pool matches the structure specified by a given description scheme.
 *
 * @section PoolByHand Accessing the pool by hand
 *
 * Accessing the pool by hand is not the ideal way of doing it but,
 * currently, extractor binding is not so complete so, by now,
 * it is the only way to do certain things.
 *
 * The scope provides interface to:
 * - Populate a given scope with a size.
 *   That is for the scope Note, telling how many notes there are.
 *   @code
 *   pool.SetNumberOfContexts("Note",60);
 *   @endcode
 * - Obtaining the attribute pool for writting, 
 *   so you will get a vector of pitches one for every note.
 *   @code
 *   CLAM::Pitch * pitches = pool.GetAttributePool<CLAM::Pitch>("Note","Pitch");
 *   @endcode
 * - Obtaining the attribute pool for reading
 *   @code
 *   const CLAM::Pitch * pitches = pool.GetReadAttributePool<CLAM::Pitch>("Note","Pitch");
 *   @endcode
 *
 * @warning A write access is required before any read access.
 * The pool will create the attribute pool memory only when
 * somebody requires to write in it.
 *
 * The access is templatized by the attribute type.
 * The pool user need not to handle generic types (void*, casts...)
 * and her code keeps typesafe. 
 * Some checking between the usage and the real type 
 * for the attribute is done on run-time.
 * So if you use a different value type an assertion will fail.
 * 
 * @section XML
 *   
 * Description data pools can be loaded or stored in XML, as any
 * other CLAM::Component, by using an CLAM::XmlStorage.
 * 
 * @code
 * // Storing a description in XML
 * CLAM::XmlStorage::Dump(pool, "DescriptionPool", "mysong.xml");
 * @endcode
 *
 * @code
 * // Recovering an XML description
 * CLAM::XmlStorage::Restore(pool, "mysong.xml");
 * @endcode
 *
 * 
 * @section ExtractorBinding Binding extractors 
 *
 * This part of the module is work on progress.
 *
 * You may use the data pool directly as a container of your extraction process.
 * But CLAM offers a functionality to encapsulate the extraction process
 * in an encapsulated object (CLAM::Extractor).
 * You can bind the extractor to compute a given attribute by
 * binding its output hook.
 * and bind it to compute a given attribute and obtaining data for the rest of attributes.
 *
 * Current implemented binding operations are Bindings on the same context, 
 * and indirection, that is, using an attribute to point another one even on a different scope.
 *
 * By now, there is no such abstract CLAM::Extractor but you can take a look
 * to some Extractors CLAMTest::CharCopierExtractor and CLAMTest::CharJoinExtractor
 *
 * @section DescriptionPoolTodo What is left to implement
 *
 * - An abstract cLAM::Extractor to derive from
 * - An special kind of extractor for scope population (how many items in a scope?)
 * - Bindings extension: relative position
 * - Solving Range and Relative bindings when outside the scope space
 * - The type system
 *   - Defining units friendly types for using them in attributes
 *   - Solving the creation of concrete Attribute specification from the type name
 * - XML Serialization for schemes
 * - Exploring new hook binding functionalities driven by real cases
 * 
 */
namespace CLAM
{


	/**
	 * @ingroup SemanticalAnalysis
	 * Represents a description schema for feature extraction.
	 * The description scheme defines the set of attributes (Attribute)
	 * to work with.
	 *
	 * A DescriptionScheme is only an specification.
	 * The real data is held in a DescriptionDataPool,
	 * a container that fits its structure to the one defined on
	 * the DescriptionScheme specification.
	 * 
	 * Attributes in a DescriptionScheme have a name and a type
	 * and they are organized in scopes.
	 * Each  scope (see DescriptionScope) defines a kind of target for the attributes:
	 * (note scope, sample scope, frame scope, phrase scope, sample scope...)
	 *
	 * You can add Attributes to the DescriptionScheme by
	 * using the Add method.
	 *
	 * @todo The DescriptionSchema should also contain
	 * which Extractors are used to compute the attributes and
	 * where the Extractors takes the data from.
	 *
	 * @see DescriptionDataPool
	 * 
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

		template < typename DataType >
		void AddAttribute(const std::string &scope, const std::string & name)
		{
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

