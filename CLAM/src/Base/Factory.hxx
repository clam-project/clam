#ifndef _Factory_hxx_
#define _Factory_hxx_

#include <map>
#include <string>

#include "Err.hxx"
//#include "Oscillator.hxx"
//#include "AudioAdder.hxx"

namespace CLAM {
	

/// Error type thrown by Factory classes (Factory, FactoryRegistry)
class ErrFactory : public Err
{
public:
	ErrFactory(const char* msg) : Err(msg)
	{}

};
	

class Factory
{
public:

	typedef Processing* (*CreatorMethod)(void);
	typedef std::string RegistryKey;

	Factory() {};
	~Factory() {};
	
	static Factory& GetInstance()	{
		static Factory theInstance;
		return theInstance;
	}


	/**
	 * Gives ownership of the new created Processing registered with
	 * the given name.
	 * It asserts that the name is in the registry.
	 */
	Processing* Create( const RegistryKey name );

	/** Gives ownership of the new created Processing registered with
	 * the given name.
	 * It throws an ErrFactory if the name isn't found in the registry.
	 */
	Processing* CreateSafe( const RegistryKey name ) throw (ErrFactory);

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

	template< typename ConcreteProcessingType>
	class Registrator
	{
	public:
		Registrator( RegistryKey key, Factory& fact ) {
			fact.AddCreator( key, Create );
		}

		Registrator( Factory& fact ) {
			ConcreteProcessingType dummy;
			fact.AddCreator( dummy.GetClassName(), Create );
		}

		Registrator( RegistryKey key ) {
			Factory::GetInstance().AddCreator( key, Create );
		}

		Registrator( ) {
			ConcreteProcessingType dummy;
			Factory::GetInstance().AddCreator( dummy.GetClassName(), Create );
		}

		static Processing* Create() {
			return new ConcreteProcessingType;
		}
	};

private:
	Registry _registry;

};



// Class definitions (not placed in the cxx because Factory will become template)



inline Processing* Factory::Create( const RegistryKey name )
{
	// it asserts that name is in the registry
	CreatorMethod creator =
		_registry.GetCreator( name );
	return (*creator)();
}


inline Processing* Factory::CreateSafe( const RegistryKey name ) throw (ErrFactory)
{
	return  _registry.GetCreatorSafe(name)();
}
inline void Factory::Clear()
{
	_registry.RemoveAllCreators();
}
inline void Factory::AddCreator(const RegistryKey name, CreatorMethod creator) {
	_registry.AddCreator(name, creator);
}

inline void Factory::AddCreatorSafe(const RegistryKey name, CreatorMethod creator) throw (ErrFactory) {
	_registry.AddCreatorSafe(name, creator);
}




inline Factory::CreatorMethod Factory::Registry::GetCreator( RegistryKey creatorId)
{
	CLAM_ASSERT(_creators.begin() != _creators.end(), 
		"the Factory Registry shouldn't be empty");

	CreatorMethod res = CommonGetCreator(creatorId);
	CLAM_ASSERT(res,"GetCreatorSafe invoked with a non existent key")

	return res;
}

inline Factory::CreatorMethod Factory::Registry::GetCreatorSafe( RegistryKey creatorId) throw (ErrFactory)
{
	if ( _creators.begin() == _creators.end() )
		throw ErrFactory("GetCreatorSafe invoked on an empty registry");

	CreatorMethod res = CommonGetCreator(creatorId);
	if (!res)
		throw ErrFactory("GetCreatorSafe invoked with a non existent key");

	return res;
}

inline void Factory::Registry::AddCreator( RegistryKey creatorId, CreatorMethod creator )
{
	if( !CommonAddCreator( creatorId, creator ) ) { 
		// repeated key
		CLAM_ASSERT( false, "creatorId was already a key in the registry" );
	}
}

inline void Factory::Registry::AddCreatorSafe( RegistryKey creatorId, CreatorMethod creator ) throw (ErrFactory)
{
	if( !CommonAddCreator( creatorId, creator ) ) { 
		// repeated key
		throw ErrFactory("A repeated key was passed");
	}
}

inline void Factory::Registry::RemoveAllCreators() 
{
	_creators.clear();
}

inline std::size_t Factory::Registry::Count()
{
	return _creators.size();
}

// helper methods:
inline Factory::CreatorMethod Factory::Registry::CommonGetCreator( RegistryKey& creatorId )
{
	CreatorMap::const_iterator i = _creators.find(creatorId);
	if ( i==_creators.end() ) {
		// not found
		return NULL;
	} else 
	return i->second;
}

inline bool Factory::Registry::CommonAddCreator( RegistryKey& creatorId, CreatorMethod creator)
{
	// returns false if the key was repeated.
	return  _creators.insert(
		CreatorMap::value_type( creatorId, creator ) ).second;
}





} // namespace

#endif // _Factory_hxx_

