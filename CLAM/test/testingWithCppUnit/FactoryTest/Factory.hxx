#ifndef _Factory_hxx_
#define _Factory_hxx_

#include <map>
#include <string>

#include "Err.hxx"
#include "Oscillator.hxx"
#include "AudioAdder.hxx"

namespace CLAM {
	
	Processing * CreateOscillator() {
		return new Oscillator;
	}

	Processing * CreateAudioAdder() {
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
private:
	typedef std::string RegistryKey;
	typedef std::map<std::string, CreatorMethod> CreatorMap;

public:
	CreatorMethod GetCreator( RegistryKey creatorId)
	{
		CreatorMethod foo = 0;//CLAM::CreateOscillator;

		CLAM_ASSERT(_creators.begin() != _creators.end(), 
			"the Factory Registry shouldn't be empty");
		
		return CommonGetCreator(creatorId);
	}

	CreatorMethod GetCreatorSafe( RegistryKey creatorId) throw (ErrFactory)
	{
		if ( _creators.begin() == _creators.end() )
			throw ErrFactory("GetCreatorSafe invoked on an empty registry");

		return CommonGetCreator(creatorId);
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




} // namespace

#endif // _Factory_hxx_

