
#include "FlowControl.hxx"

#include <iostream>
#include "Network.hxx"
#include "Processing.hxx"

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
	//added new connection to network
	if(_state != SomeNodesNotConfigured )
		return;

	Network::NodesIterator it;
	for (it = _network->BeginNodes(); it != _network->EndNodes(); it++ )
	{
		(*it)->Configure(_frameSize);
	}
	_state = Ready;
}

void FlowControl::ConfigurePorts()
{
	// added new processing to network
	if(_state != SomePortsNotConfigured )
		return;

	Network::ProcessingsMapIterator it;
	for( it=_network->BeginProcessings(); it!= _network->EndProcessings(); it++)
	{
		Processing* proc = it->second;
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
	_state = Ready;
}

void FlowControl::ProcessingAddedToNetwork()
{
	_state = SomePortsNotConfigured;
}

void FlowControl::ConnectionAddedToNetwork()
{
	_state = SomeNodesNotConfigured;
}

void FlowControl::StartNetwork()
{
	CLAM_ASSERT(_state == Ready , "FlowControl not ready to Start Network");

	Network::ProcessingsMapIterator it;
	for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
	{
		it->second->Start();
	}
	_state = Running;
}

void FlowControl::StopNetwork()
{
	CLAM_ASSERT(_state == Running , "FlowControl cannot Stop a Network that is not running");

	Network::ProcessingsMapIterator it;
	for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
	{
		it->second->Stop();
	}
	_state = Ready;
}

void FlowControl::DoProcessings()
{
	CLAM_ASSERT(_state == Running , "FlowControl not started");		
	Network::ProcessingsMapIterator it;
	for ( it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++ )
	{
		if (it->second->CanDoUsingPorts())
			it->second->Do();
	}
}

}
