#ifndef _Network_hxx_
#define _Network_hxx_

#include "Processing.hxx"
#include "Node.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "OutPortTmpl.hxx"
#include "CircularStreamImpl.hxx"
#include "NodeTmpl.hxx"
#include <string>
#include <map>

// to remove
#include "DummyProcessingData.hxx"
#include "Audio.hxx"

namespace CLAM
{


class Network
{
public:
	typedef std::map< std::string, Processing* > ProcessingsMap;
	typedef std::list< NodeBase* > NodesList;
	// destructor
	~Network();

	Processing& GetProcessing( const std::string & name );
	void AddProcessing( const std::string &, Processing* );
	bool HasProcessing( const std::string & name );
	bool ConnectPorts( const std::string &, const std::string & );
	NodeBase& GetNode(OutPort& out);

protected:
	InPort & GetInPortByCompleteName( const std::string& );
	OutPort & GetOutPortByCompleteName( const std::string& );
	InControl & GetInControlByCompleteName( const std::string& );
	OutControl & GetOutControlByCompleteName( const std::string& );

//	template <typename OutPortType>
	NodeBase& GetNode(OutPort & out)
	{

		NodeTmpl<OutPort::PortType,
			CircularStreamImpl<OutPort::PortType> > node;
//		typedef typename CircularStreamImpl<DummyProcessingData> ConcreteBuffer;
//		typedef typename CircularStreamImpl<typename OutPortType::PortType> ConcreteBuffer;
//		typedef typename NodeTmpl<<OutPortType::PortType, <CircularStreamImpl<OutPortType::PortType>>> ConcreteNodeType;
		

		if (out.GetNode() == 0) //if it hasn't associated node
		{

//			_nodes.insert(node);
//			out.Attach(node);
		}
		return *(out.GetNode());
	}
	

private:
	// fields
	ProcessingsMap _processings;
	NodesList _nodes;
	// helpers
	static std::size_t PositionOfLastIdentifier( const std::string& );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );
	std::string GetProcessingIdentifier( const std::string& );
	static const char NamesIdentifiersSeparator();
	
};

}// namespace
#endif
