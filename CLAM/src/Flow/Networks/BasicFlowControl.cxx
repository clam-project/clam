

#include "BasicFlowControl.hxx"
#include "Network.hxx"
#include "Processing.hxx"

namespace CLAM
{

BasicFlowControl::BasicFlowControl( int frameSize )
	: FlowControl( frameSize )
{
}

void BasicFlowControl::DoProcessings()
{
	CLAM_ASSERT(_state == Running , "FlowControl not started");		

	Network::ProcessingsMap::iterator it;
	for ( it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++ )
	{
		if (it->second->CanDoUsingPorts())
			it->second->Do();
	}
	
}

} // namespace CLAM
