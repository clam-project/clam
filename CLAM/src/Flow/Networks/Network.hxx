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
	
	void Start();
	void Stop();
	void DoProcessings();
	void ConfigureNodes( int frameSize );

protected:
	InPort & GetInPortByCompleteName( const std::string& );
	OutPort & GetOutPortByCompleteName( const std::string& );
	InControl & GetInControlByCompleteName( const std::string& );
	OutControl & GetOutControlByCompleteName( const std::string& );
	NodeBase & GetNodeAttachedTo(OutPort & );

private:
	// fields
	ProcessingsMap _processings;
	NodesList _nodes;
	// helpers
	static std::size_t PositionOfLastIdentifier( const std::string& );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );
	std::string GetProcessingIdentifier( const std::string& );
	static char NamesIdentifiersSeparator();

	/**this method is provisional, because Network may need non-audio nodes.
	 * Thus the factory method should be a (virtual) method of OutPort, implemented
	 * in the concrete class.
	 * \todo the motivation for this kludge is in order to avoid the coupling between
	 * ports and the nodes stuff, since VC6 doesn't compiles all of it.
	 */
	NodeBase* CreateAudioNodeWithDefaultStreamBuffer();
	
};

}// namespace
#endif
