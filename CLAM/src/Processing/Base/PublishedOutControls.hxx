#ifndef _PublishedOutControls_hxx_
#define _PublishedOutControls_hxx_

#include "OutControl.hxx"

namespace CLAM
{
//foward declaration:
class Processing;

class PublishedOutControls
{
public:
	PublishedOutControls(Processing* parent);

	OutControl& GetByNumber(int index) const;
	OutControl& Get(std::string name) const;
	int Size() const;

private:
	Processing& mParent;
};


} // namespace CLAM

#endif
