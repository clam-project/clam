
#include "InPort.hxx"
#include "OutPort.hxx"

namespace CLAM
{

InPortBase::InPortBase() : mAttachedOutPort(0)
{
}

InPortBase::~InPortBase()
{
}

OutPortBase * InPortBase::GetAttachedOutPort() 
{
	return mAttachedOutPort;
}
	
void InPortBase::DisconnectAndUnAttach()
{	
	CLAM_DEBUG_ASSERT(mAttachedOutPort, "InPortBase::DisconnectAndUnAttach() - InPort is not connected" );
	if(mAttachedOutPort)
		mAttachedOutPort->DisconnectFromIn( *this );
}

} // namespace CLAM
