#include <Oscillator.hxx>
#include <map>
#include <string>

#include "Err.hxx"

namespace CLAM {
	
	Processing * CreateOscillator() {
		return new Oscillator;
	}

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
	void GetCreatorSafe(char*)
	{
		return;
	}
private:
	std::map< RegistryKey, CreatorMethod> _creators;
};


class ErrFactory : public Err
{
};

} // namespace


