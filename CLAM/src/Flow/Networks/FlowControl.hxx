
#ifndef _FlowControl_hxx_
#define _FlowControl_hxx_

namespace CLAM
{

class Network;

class FlowControl
{
	typedef enum {
		SomePortsNotConfigured,
		SomeNodesNotConfigured,
		Ready,
		Running		
	} ExecState;
public:
	FlowControl( Network &, int frameSize = 0 );
	virtual ~FlowControl(){}
	void ConfigureNodes();
	void ConfigurePorts();
	
	//methods relative to state of FlowControl & Network
	void ProcessingAddedToNetwork();
	void ConnectionAddedToNetwork();

	void StartNetwork();
	void StopNetwork();
	void DoProcessings();
private:
	
	int _frameSize;
	Network & _network;
	ExecState _state;
};

}

#endif
