#ifndef _Factory_hxx_
#define _Factory_hxx_

#include <map>
#include <string>

#include "Err.hxx"

namespace CLAM {
class Processing;

/// Error type thrown by Factory classes (Factory, FactoryRegistry)
class ErrFactory : public Err
{
public:
	ErrFactory(const char* msg) : Err(msg)
	{}

};


template <typename AbstractProductType = Processing>
class Factory
{
public:

	typedef AbstractProductType AbstractProduct;
	typedef AbstractProduct* (*CreatorMethod)(void);
	typedef std::string RegistryKey;

	Factory() {};
	~Factory() {};

	static Factory& GetInstance()	{
		static Factory theInstance;
		return theInstance;
	}


	/**
	 * Gives ownership of the new created Product registered with
	 * the given name.
	 * It asserts that the name is in the registry.
	 */
	AbstractProduct* Create( const RegistryKey name );

	/** Gives ownership of the new created Product registered with
	 * the given name.
	 * It throws an ErrFactory if the name isn't found in the registry.
	 */
	AbstractProduct* CreateSafe( const RegistryKey name ) throw (ErrFactory);

	void Clear();

	void AddCreator(const RegistryKey name, CreatorMethod creator);

	void AddCreatorSafe(const RegistryKey name, CreatorMethod creator) throw (ErrFactory);


public: // Inner classes. Public for better testing

	class Registry
	{
	private:
		typedef std::map<std::string, CreatorMethod> CreatorMap;

	public:
		CreatorMethod GetCreator( RegistryKey creatorId);
		CreatorMethod GetCreatorSafe( RegistryKey creatorId) throw (ErrFactory);

		void AddCreator( RegistryKey creatorId, CreatorMethod creator );
		void AddCreatorSafe( RegistryKey creatorId, CreatorMethod creator ) throw (ErrFactory);

		void RemoveAllCreators();
		std::size_t Count();

	private: // data
		CreatorMap _creators;

		// helper methods:
		CreatorMethod CommonGetCreator( RegistryKey& creatorId );
		bool CommonAddCreator( RegistryKey& creatorId, CreatorMethod creator);
	};

	template< typename ConcreteProductType>
	class Registrator
	{
	public:
		Registrator( RegistryKey key, Factory& fact ) {
			fact.AddCreator( key, Create );
		}

		Registrator( Factory& fact ) {
			ConcreteProductType dummy;
			fact.AddCreator( dummy.GetClassName(), Create );
		}

		Registrator( RegistryKey key ) {
			Factory::GetInstance().AddCreator( key, Create );
		}

		Registrator( ) {
			ConcreteProductType dummy;
			Factory::GetInstance().AddCreator( dummy.GetClassName(), Create );
		}

		static AbstractProduct* Create() {
			return new ConcreteProductType;
		}
	};

private:
	Registry _registry;

};



// Class definitions (not placed in the cxx because Factory will become template)


template <typename AbstractProductType>
inline typename Factory<AbstractProductType>::AbstractProduct*
	Factory<AbstractProductType>::Create( const RegistryKey name )
{
	// it asserts that name is in the registry
	CreatorMethod creator =
		_registry.GetCreator( name );
	return (*creator)();
}


template <typename AbstractProductType>
inline typename Factory<AbstractProductType>::AbstractProduct*
	Factory<AbstractProductType>::CreateSafe( const RegistryKey name ) throw (ErrFactory)
{
	return  _registry.GetCreatorSafe(name)();
}

template <typename AbstractProductType>
inline void Factory<AbstractProductType>::Clear()
{
	_registry.RemoveAllCreators();
}

template <typename AbstractProductType>
inline void Factory<AbstractProductType>::AddCreator(const RegistryKey name, CreatorMethod creator) {
	_registry.AddCreator(name, creator);
}

template <typename AbstractProductType>
inline void Factory<AbstractProductType>::AddCreatorSafe(const RegistryKey name, CreatorMethod creator) throw (ErrFactory) {
	_registry.AddCreatorSafe(name, creator);
}



template <typename AbstractProductType>
inline typename Factory<AbstractProductType>::CreatorMethod 
	Factory<AbstractProductType>::Registry::GetCreator( RegistryKey creatorId)
{
	CLAM_ASSERT(_creators.begin() != _creators.end(),
		"the Factory Registry shouldn't be empty");

	CreatorMethod res = CommonGetCreator(creatorId);
	CLAM_ASSERT(res,"GetCreatorSafe invoked with a non existent key")

	return res;
}

template <typename AbstractProductType>
inline typename Factory<AbstractProductType>::CreatorMethod 
	Factory<AbstractProductType>::Registry::GetCreatorSafe( RegistryKey creatorId) throw (ErrFactory)
{
	if ( _creators.begin() == _creators.end() )
		throw ErrFactory("GetCreatorSafe invoked on an empty registry");

	CreatorMethod res = CommonGetCreator(creatorId);
	if (!res)
		throw ErrFactory("GetCreatorSafe invoked with a non existent key");

	return res;
}

template <typename AbstractProductType>
inline void Factory<AbstractProductType>::Registry::AddCreator( RegistryKey creatorId, CreatorMethod creator )
{
	if( !CommonAddCreator( creatorId, creator ) ) {
		// repeated key
		CLAM_ASSERT( false, "creatorId was already a key in the registry" );
	}
}

template <typename AbstractProductType>
inline void Factory<AbstractProductType>::Registry::AddCreatorSafe( RegistryKey creatorId, CreatorMethod creator ) throw (ErrFactory)
{
	if( !CommonAddCreator( creatorId, creator ) ) {
		// repeated key
		throw ErrFactory("A repeated key was passed");
	}
}

template <typename AbstractProductType>
inline void Factory<AbstractProductType>::Registry::RemoveAllCreators()
{
	_creators.clear();
}

template <typename AbstractProductType>
inline std::size_t Factory<AbstractProductType>::Registry::Count()
{
	return _creators.size();
}

// helper methods:
template <typename AbstractProductType>
inline typename Factory<AbstractProductType>::CreatorMethod Factory<AbstractProductType>::Registry::CommonGetCreator( RegistryKey& creatorId )
{
	CreatorMap::const_iterator i = _creators.find(creatorId);
	if ( i==_creators.end() ) {
		// not found
		return NULL;
	} else
	return i->second;
}

template <typename AbstractProductType>
inline bool Factory<AbstractProductType>::Registry::CommonAddCreator( RegistryKey& creatorId, CreatorMethod creator)
{
	// returns false if the key was repeated.
	return  _creators.insert(
		CreatorMap::value_type( creatorId, creator ) ).second;
}





} // namespace

#endif // _Factory_hxx_

