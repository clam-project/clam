
#include "FlowControl.hxx"

#include <iostream>
#include "Network.hxx"
#include "Processing.hxx"
#include "Node.hxx"

namespace CLAM
{

FlowControl::FlowControl(  int frameSize )
	:_frameSize(frameSize),
	 _network(0)	 
{
	_state = NotAttachedToNetwork;
}      

void FlowControl::AttachToNetwork( Network* network)
{
	_network = network;
	_state = Ready;
}

void FlowControl::ConfigureNodes()
{
	// added new processing to network
	if(_unconfiguredNodes.empty())
		return;

	Network::Nodes::iterator it;
	for ( it=_unconfiguredNodes.begin(); it!=_unconfiguredNodes.end(); it++ )
	{
		(*it)->Configure(_frameSize);
	}
	_unconfiguredNodes.clear();
}

void FlowControl::ConfigurePorts()
{
	// added new processing to network
	if(_unconfiguredProcessings.empty())
		return;

	std::list< Processing* >::iterator it;
	for( it=_unconfiguredProcessings.begin(); it!= _unconfiguredProcessings.end(); it++)
	{
		Processing* proc = (*it);
		Processing::InPortIterator itin;
		for (itin = proc->GetInPorts().Begin(); 
		     itin != proc->GetInPorts().End(); 
		     itin++)
			(*itin)->SetParams(_frameSize);
		
		Processing::OutPortIterator itout;
		for (itout = proc->GetOutPorts().Begin(); 
		     itout != proc->GetOutPorts().End(); 
		     itout++)
			(*itout)->SetParams(_frameSize);		
	}
	_unconfiguredProcessings.clear();
}

void FlowControl::ProcessingAddedToNetwork( Processing* proc )
{
	_unconfiguredProcessings.push_back(proc);
}

void FlowControl::NodeAddedToNetwork( NodeBase * node )
{
	_unconfiguredNodes.push_back(node);
}

void FlowControl::StartNetwork()
{
	CLAM_ASSERT(_state == Ready , "FlowControl not ready to Start Network");

	Network::ProcessingsMap::iterator it;
	for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
	{
		it->second->Start();
	}
	_state = Running;
}

void FlowControl::StopNetwork()
{
	CLAM_ASSERT(_state == Running , "FlowControl cannot Stop a Network that is not running");

	Network::ProcessingsMap::iterator it;
	for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
	{
		it->second->Stop();
	}
	_state = Ready;
}

void FlowControl::DoProcessings()
{
	CLAM_ASSERT(_state == Running , "FlowControl not started");		

	Network::ProcessingsMap::iterator it;
	for ( it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++ )
	{
		if (it->second->CanDoUsingPorts())
			it->second->Do();
	}
}


}
