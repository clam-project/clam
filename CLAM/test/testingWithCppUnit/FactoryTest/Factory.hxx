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
	typedef Processing* (*CreatorMethod)(void);
	typedef std::string RegistryKey;
	typedef std::map<std::string, CreatorMethod> CreatorMap;

public:
	CreatorMethod GetCreator(char*)
	{
		CreatorMethod foo = 0;//CLAM::CreateOscillator;

		CLAM_ASSERT(_creators.begin() != _creators.end(), 
			"the Factory Registry shouldn't be empty");
		
		return 0;
	}
	CreatorMethod GetCreatorSafe(char*) throw (ErrFactory)
	{
		if ( _creators.begin() == _creators.end() )
			throw ErrFactory("GetCreatorSafe invoked on an empty registry");

		return 0;
	}
	void AddCreator( RegistryKey creatorId, CreatorMethod creator )
	{
		return; // fake impl.
	}
private:
	CreatorMap _creators;
};




} // namespace


