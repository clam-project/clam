
#include "Network.hxx"
#include <algorithm>

namespace CLAM
{
	namespace HelperFunctions
	{
		void DeleteProcessing( Network::ProcessingsMap::value_type& mapElem ) {
			delete mapElem.second;
		}
		void StartProcessing( Network::ProcessingsMap::value_type& mapElem ) {
			mapElem.second->Start();
		}
		void StopProcessing( Network::ProcessingsMap::value_type& mapElem ) {
			mapElem.second->Stop();
		}
	}
	// constructor / destructor

	Network::Network( const std::string& name ) :
		_name( name )
	{}

	Network::Network() :
		_name("Unnamed Network")
	{}
	
	Network::~Network()
	{
		std::for_each( 	_processings.begin(), _processings.end(), HelperFunctions::DeleteProcessing );
	}

	Processing& Network::GetProcessing( const std::string & name )
	{
		CLAM_ASSERT( HasProcessing(name), "No Processing with the given name" );

		ProcessingsMap::const_iterator it = _processings.find( name );
		return *it->second;
	}

	void Network::AddProcessing( const std::string & name, Processing* proc)
	{
		// returns false if the key was repeated.
		if (!_processings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );
	}

	bool Network::HasProcessing( const std::string & name )
	{
		ProcessingsMap::const_iterator i = _processings.find( name );
		return i!=_processings.end();
	}


	bool Network::ConnectPorts( const std::string & producer, const std::string & consumer )
	{
		OutPort & outport = GetOutPortByCompleteName(producer);
		InPort & inport = GetInPortByCompleteName(consumer);

		if ( outport.IsConnectedTo(inport) ) 
			return true;
			
		if ( !outport.IsConnectableTo(inport) ) //they have different type
			return false;

		inport.Attach(GetNodeAttachedTo(outport));
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
		typedef CircularStreamImpl<TData> DefaultStreamBuffer;
		return new NodeTmpl<Audio, DefaultStreamBuffer>;
	}

	void Network::Start()
	{
		
		std::for_each( _processings.begin(), _processings.end(), HelperFunctions::StartProcessing );
	}
	void Network::Stop()
	{
		std::for_each( _processings.begin(), _processings.end(), HelperFunctions::StopProcessing );
	}
	void Network::DoProcessings()
	{
		ProcessingsMap::iterator it;
		for ( it=_processings.begin(); it!=_processings.end(); it++ )
				it->second->Do();
	}

	void Network::ConfigureNodes( int frameSize )
	{
		NodesList::iterator it;
		for (it = _nodes.begin(); it != _nodes.end(); it++ )
		{
			(*it)->Configure(frameSize);
		}
	}

}
