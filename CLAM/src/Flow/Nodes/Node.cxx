
#include "Node.hxx"
#include <vector>

namespace CLAM
{

const OutPort* NodeBase::GetWriter() const
{
	return mpDriver;
}

const std::list<InPort*> NodeBase::GetReaders() const
{
	std::list<InPort*> listReaders;
	
	for(int i=0; i<mInputs.Size(); i++)
	{
		listReaders.push_back(mInputs[i]);
	}
	return listReaders;
}

} // namespace CLAM
