#ifndef _PublishedInControls_hxx_
#define _PublishedInControls_hxx_

#include "InControl.hxx"

namespace CLAM
{
//foward declaration:
class Processing;

class PublishedInControls
{
public:
	PublishedInControls(Processing* parent);

	InControl& GetByNumber(int index) const;
	InControl& Get(std::string name) const;
	int Size() const;

private:
	Processing& mParent;
};


} // namespace CLAM
#endif
