#include <Oscillator.hxx>

namespace CLAM {
	
	Processing * CreateOscillator() {
		return new Oscillator;
	}

class FactoryRegistry
{	
public:
	void GetCreator(char* id)
	{
		CLAM_ASSERT(false, "id should be in the registry");

	}
};

} // namespace


