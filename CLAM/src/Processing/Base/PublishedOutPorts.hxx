#ifndef _PublishedOutPorts_hxx_
#define _PublishedOutPorts_hxx_

#include <string>

namespace CLAM
{

//foward declarations:
class Processing;
class OutPort;

class PublishedOutPorts
{
public:
	PublishedOutPorts(Processing* parent);

	OutPort& GetByNumber(int index) const;
	OutPort& Get(std::string name) const;
	int Size() const;

private:
	Processing& mParent;
};

} // namespace CLAM

#endif
