#ifndef _PublishedInPorts_hxx_
#define _PublishedInPorts_hxx_
#include <string>

namespace CLAM
{

//foward declarations:
class Processing;
class InPort;

class PublishedInPorts
{
public:
	PublishedInPorts(Processing* parent);

	InPort& GetByNumber(int index) const;
	InPort& Get( std::string name ) const;
	int Size() const;

private:
	Processing& mParent;
};

} // namespace CLAM

#endif
