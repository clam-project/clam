
#include "Network.hxx"
#include "FlowControl.hxx"
#include <algorithm>
#include <iostream>

namespace CLAM
{
	namespace HelperFunctions
	{
		void DeleteProcessing( Network::ProcessingsMap::value_type& mapElem ) {
			delete mapElem.second;
		}
	}
	// constructor / destructor

	Network::Network( const std::string& name ) :
		_name( name ),
		_flowControl(0)
	{}   

	Network::Network() :
		_name("Unnamed Network"),
		_flowControl(0)
	{}
	
	Network::~Network()
	{
		if (_flowControl)
			delete _flowControl;

		std::for_each( 	_processings.begin(), _processings.end(), HelperFunctions::DeleteProcessing );
	}

	/** Gets the ownership of the FlowControl passed. So it will be deleted by the destructor */
	void Network::AddFlowControl(FlowControl* flowControl)
	{		
		_flowControl = flowControl;
		_flowControl->AttachToNetwork(this);
	}

	Processing& Network::GetProcessing( const std::string & name )
	{
		CLAM_ASSERT( HasProcessing(name), "No Processing with the given name" );

		ProcessingsMap::const_iterator it = _processings.find( name );
		return *it->second;
	}

	void Network::AssertFlowControlNotNull() const
	{
		CLAM_ASSERT( 
			_flowControl, 
			"the Network should have a FlowControl. Use Network::AddFlowControl(FlowControl*)");
	}

	void Network::AddProcessing( const std::string & name, Processing* proc)
	{
//		AssertFlowControlNotNull();

		// returns false if the key was repeated.
		if (!_processings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );

		if(_flowControl)
			_flowControl->ProcessingAddedToNetwork();
	}

	bool Network::HasProcessing( const std::string & name )
	{
		ProcessingsMap::const_iterator i = _processings.find( name );
		return i!=_processings.end();
	}


	bool Network::ConnectPorts( const std::string & producer, const std::string & consumer )
	{
//		AssertFlowControlNotNull();

		OutPort & outport = GetOutPortByCompleteName(producer);
		InPort & inport = GetInPortByCompleteName(consumer);

		if ( outport.IsConnectedTo(inport) ) 
			return false;
			
		if ( !outport.IsConnectableTo(inport) ) //they have different type
			return false;

		if( inport.IsAttached())
			return false;

		inport.Attach(GetNodeAttachedTo(outport));
		
		if(_flowControl)
			_flowControl->ConnectionAddedToNetwork();

		return true;
	}

	bool Network::RemovePortsConnection( const std::string & producer, const std::string & consumer)
	{
		OutPort & outport = GetOutPortByCompleteName(producer);
		InPort & inport = GetInPortByCompleteName(consumer);

		if ( !outport.IsConnectedTo(inport) ) 
			return false;

		inport.Unattach();
		
		//todo: send a message to flowcontrol about connections modified

		return true;
	}


	char Network::NamesIdentifiersSeparator()
	{ 	
		return '.'; 	
	}

	std::size_t Network::PositionOfLastIdentifier( const std::string & str ) 
	{
		std::size_t result = str.find_last_of( NamesIdentifiersSeparator() );
		CLAM_ASSERT( result!=std::string::npos, "Malformed port name. It should be ProcessingName.[Port/Control]Name");
		return result;
	}
	std::size_t Network::PositionOfProcessingIdentifier( const std::string& str )
	{
		std::size_t endPos = PositionOfLastIdentifier(str)-1;
		std::size_t	last_ofResult = str.find_last_of( NamesIdentifiersSeparator(), endPos );
		return last_ofResult == std::string::npos ? 0 : last_ofResult+1;
	}

	std::string Network::GetLastIdentifier( const std::string& str )
	{
		return str.substr( PositionOfLastIdentifier(str)+1 );
	}

	std::string Network::GetProcessingIdentifier( const std::string& str )
	{
		std::size_t length = PositionOfLastIdentifier(str)  - PositionOfProcessingIdentifier(str);
		return str.substr( PositionOfProcessingIdentifier(str), length);
	}

	InPort & Network::GetInPortByCompleteName( const std::string & name )
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInPorts().Get( GetLastIdentifier(name) );
	}

	OutPort & Network::GetOutPortByCompleteName( const std::string & name )
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutPorts().Get( GetLastIdentifier(name) );
	}

	InControl & Network::GetInControlByCompleteName( const std::string & name )
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInControls().Get( GetLastIdentifier(name) );
	}

	OutControl & Network::GetOutControlByCompleteName( const std::string & name )
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutControls().Get( GetLastIdentifier(name) );
	}

	NodeBase & Network::GetNodeAttachedTo( OutPort & out )
	{
			if (!out.GetNode())
			{
				NodeBase * node = CreateAudioNodeWithDefaultStreamBuffer();
				out.Attach(*node);
				_nodes.push_back(node);				
			}
			return *out.GetNode();
	}

	NodeBase* Network::CreateAudioNodeWithDefaultStreamBuffer()
	{
		//@todo
		typedef CircularStreamImpl<TData> DefaultStreamBuffer;
		return new NodeTmpl<Audio, DefaultStreamBuffer>;
		//return new AudioNodeTmpl;
	}

	void Network::Start()
	{
		AssertFlowControlNotNull();
		_flowControl->StartNetwork();
	}
	void Network::Stop()
	{
		AssertFlowControlNotNull();
		_flowControl->StopNetwork();
	}
	void Network::DoProcessings()
	{
		AssertFlowControlNotNull();
		_flowControl->DoProcessings();
	}

	void Network::ConfigureNodes( int frameSize )
	{
		AssertFlowControlNotNull();
		_flowControl->ConfigureNodes();
	}

	void Network::ConfigurePorts( int frameSize )
	{
		AssertFlowControlNotNull();
		_flowControl->ConfigurePorts();	
	}

	Network::ProcessingsMapIterator Network::BeginProcessings()
	{
		return _processings.begin();
	}

	Network::ProcessingsMapIterator Network::EndProcessings()
	{
		return _processings.end();
	}
	Network::ConstProcessingsMapIterator Network::BeginProcessings() const
	{
		return _processings.begin();
	}

	Network::ConstProcessingsMapIterator Network::EndProcessings() const
	{
		return _processings.end();
	}

	Network::NodesIterator Network::BeginNodes()
	{
		return _nodes.begin();
	}

	Network::NodesIterator Network::EndNodes()
	{
		return _nodes.end();
	}

	Network::ConstNodesIterator Network::BeginNodes() const
	{
		return _nodes.begin();
	}

	Network::ConstNodesIterator Network::EndNodes() const
	{
		return _nodes.end();
	}


}
