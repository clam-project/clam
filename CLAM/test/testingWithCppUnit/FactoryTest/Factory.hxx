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

public:
	void GetCreator(char*)
	{
		CLAM_ASSERT(_creators.begin() != _creators.end(), 
			"the Factory Registry shouldn't be empty");
	}
	void GetCreatorSafe(char*) throw (ErrFactory)
	{
		if ( _creators.begin() == _creators.end() )
			throw ErrFactory("GetCreatorSafe invoked on an empty registry");
	}
private:
	std::map< RegistryKey, CreatorMethod> _creators;
};




} // namespace


