#include <Oscillator.hxx>
#include <map>
#include <string>

#include "Err.hxx"

namespace CLAM {
	
	Processing * CreateOscillator() {
		return new Oscillator;
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
		
		CreatorMap::const_iterator i = _creators.find(creatorId);

		return i->second;
	}
	CreatorMethod GetCreatorSafe(char*) throw (ErrFactory)
	{
		if ( _creators.begin() == _creators.end() )
			throw ErrFactory("GetCreatorSafe invoked on an empty registry");

		return 0;
	}
	void AddCreator( RegistryKey creatorId, CreatorMethod creator )
	{
		_creators.insert( 
			CreatorMap::value_type( creatorId, creator ) );
	}
private:
	CreatorMap _creators;
};




} // namespace


