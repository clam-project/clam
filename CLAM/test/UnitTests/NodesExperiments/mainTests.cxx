#include "TestsPortsRegionsInteraction.cxx" //TODO: do it properly. maybe with cppunit
#include "TestsStream.cxx"
#include "TestsPhantomBufferStream.cxx"
#include "TestsStlBasedStream.cxx"
#include "TestsRandomStream.cxx"

#include <list>
#include <vector>
#include "PhantomBuffer.hxx"


template <typename T>
class StdVector : public std::vector<T>
{};

int main()
{

	TestsPortsRegionsInteraction().testeja();
	TestsStream< CLAM::PhantomBuffer >().testeja();
	TestsStream< StdVector >().testeja();
	TestsStream< StdList >().testeja();

	TestsPhantomBufferStream().testeja();
	TestsStlBasedStream().testeja();
	TestsRandomStream< CLAM::PhantomBuffer >().testeja();


	return 0;
}

