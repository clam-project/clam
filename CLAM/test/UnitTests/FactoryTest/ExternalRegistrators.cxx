#include "Factory.hxx"
#include "DummyProducts.hxx"

namespace CLAMTest
{

typedef CLAM::Factory< A > FactoryOfAs;

	FactoryOfAs* gAddressOfFactoryUsedByRegistrator;

static FactoryOfAs::Registrator<B> DummyRegt("B", gAddressOfFactoryUsedByRegistrator );

}
