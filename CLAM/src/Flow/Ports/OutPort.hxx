#ifndef _OutPort_hxx_
#define _OutPort_hxx_

#include "Port.hxx"
#include <string>

namespace CLAM
{

class Processing;
class InPort;
class NodeBase;

class OutPort: public Port {
public:
	OutPort(const std::string &n, Processing *o, int length, int hop = 0);
	virtual bool IsConnectableTo(InPort & ) = 0;
	virtual NodeBase* CreateNodeWithDefaultStreamBuffer() = 0;

	virtual ~OutPort() {}

};

inline OutPort::OutPort(const std::string &n,
				 Processing *o,
				 int length,
				 int hop ) : 
	Port(n,o,length,hop)
{}

} // namespace CLAM

#endif
