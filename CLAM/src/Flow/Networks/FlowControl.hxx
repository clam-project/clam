
#ifndef _FlowControl_hxx_
#define _FlowControl_hxx_


#include <list>

namespace CLAM
{

class Network;
class NodeBase;
class Processing;

class FlowControl
{
protected:
	typedef enum {
		NotAttachedToNetwork,
		Ready,
		Running		
	} ExecState;
public:
	FlowControl( int frameSize = 0 );
	virtual ~FlowControl(){}
	virtual void AttachToNetwork( Network* );
	virtual void ConfigureNodes();
	virtual void ConfigurePorts();
	
	//methods relative to state of FlowControl & Network
	virtual void ProcessingAddedToNetwork( Processing * );
	virtual void NodeAddedToNetwork( NodeBase * );

	virtual void StartNetwork();
	virtual void StopNetwork();

	// to implement in each type of flowcontrol
	virtual void DoProcessings() = 0;
protected:
	
	int _frameSize;
	Network * _network;
	ExecState _state;
	
	std::list< NodeBase* > _unconfiguredNodes;
	std::list< Processing* > _unconfiguredProcessings;
};

}

#endif
