#ifndef _Factory_hxx_
#define _Factory_hxx_

#include <map>
#include <string>

#include "Err.hxx"
#include "Oscillator.hxx"
#include "AudioAdder.hxx"

namespace CLAM {
	
	inline Processing * CreateOscillator() {
		return new Oscillator;
	}

	inline Processing * CreateAudioAdder() {
		return new AudioAdder;
	}

/// Error type thrown by Factory classes (Factory, FactoryRegistry)
class ErrFactory : public Err
{
public:
	ErrFactory(const char* msg) : Err(msg)
	{}

};
	
class FactoryRegistry
{	
public:
	typedef Processing* (*CreatorMethod)(void);
	typedef std::string RegistryKey;
private:
	typedef std::map<std::string, CreatorMethod> CreatorMap;

public:
	CreatorMethod GetCreator( RegistryKey creatorId)
	{
		CLAM_ASSERT(_creators.begin() != _creators.end(), 
			"the Factory Registry shouldn't be empty");
		
		CreatorMethod res = CommonGetCreator(creatorId);
		CLAM_ASSERT(res,"GetCreatorSafe invoked with a non existent key")
			
		return res;
	}

	CreatorMethod GetCreatorSafe( RegistryKey creatorId) throw (ErrFactory)
	{
		if ( _creators.begin() == _creators.end() )
			throw ErrFactory("GetCreatorSafe invoked on an empty registry");

		CreatorMethod res = CommonGetCreator(creatorId);
		if (!res)
			throw ErrFactory("GetCreatorSafe invoked with a non existent key");
		
		return res;
	}

	void AddCreator( RegistryKey creatorId, CreatorMethod creator )
	{
		if( !CommonAddCreator( creatorId, creator ) ) { 
			// repeated key
			CLAM_ASSERT( false, "creatorId was already a key in the registry" );
		}
	}

	void AddCreatorSafe( RegistryKey creatorId, CreatorMethod creator ) throw (ErrFactory)
	{
		if( !CommonAddCreator( creatorId, creator ) ) { 
			// repeated key
			throw ErrFactory("FactoryRegistry::AddCreatorSafe(...) a repeated key was passed");
		}
	}

	void RemoveAllCreators()
	{
		_creators.clear();
	}

	std::size_t Count()
	{
		return _creators.size();
	}
	

private: // data
	CreatorMap _creators;
	
	// helper methods:
	CreatorMethod CommonGetCreator( RegistryKey& creatorId )
	{
		CreatorMap::const_iterator i = _creators.find(creatorId);
		if ( i==_creators.end() ) {
			// not found
			return NULL;
		} else 
			return i->second;
	}

	bool CommonAddCreator( RegistryKey& creatorId, CreatorMethod creator)
	{
		// returns false if the key was repeated.
		return  _creators.insert(
			CreatorMap::value_type( creatorId, creator ) ).second;

	}
	
};



class Factory
{
public:
	typedef FactoryRegistry::RegistryKey RegistryKey;
	typedef FactoryRegistry::CreatorMethod CreatorMethod;


	Factory() {};
	~Factory() {};
	
	static Factory& GetInstance()
	{
		static Factory theInstance;
		return theInstance;
	}

	/// Gives ownership of the new created Processing registered with
	/// the given name.
	/// It asserts that the name is in the registry.
	Processing* Create( const RegistryKey name )
	{
		// it asserts that name is in the registry
		CreatorMethod creator =
			_registry.GetCreator( name );
		return (*creator)();
	}

	/// Gives ownership of the new created Processing registered with
	/// the given name.
	/// It throws an ErrFactory if the name isn't found in the registry.
	Processing* CreateSafe( const RegistryKey name ) throw (ErrFactory)
	{
		return  _registry.GetCreatorSafe(name)();
	}
	void Clear()
	{
		_registry.RemoveAllCreators();
	}
	void AddCreator(const RegistryKey name, CreatorMethod creator) {
		_registry.AddCreator(name, creator);
	}

	void AddCreatorSafe(const RegistryKey name, CreatorMethod creator) throw (ErrFactory) {
		_registry.AddCreatorSafe(name, creator);
	}

private:
	FactoryRegistry _registry;

public: // Inner class

	template< typename ConcreteProcessingType>
	class Registrator
	{
	public:
		Registrator( Factory::RegistryKey key, Factory& fact )
		{
			fact.AddCreator( key, Create );
		}

		Registrator( Factory& fact )
		{
			fact.AddCreator( "Oscilator", CreateAudioAdder );
		}

		static Processing* Create()
		{
			return new ConcreteProcessingType;
		}
	};

};




} // namespace

#endif // _Factory_hxx_

