
#include "Network.hxx"
#include <algorithm>

namespace CLAM
{
	namespace Helper
	{
		void DeleteProcessing( Network::ProcessingsMap::value_type& mapElem ) {
			delete mapElem.second;
		}
	}
	// destructor
	Network::~Network()
	{
		std::for_each(_processings.begin(),	_processings.end(),	Helper::DeleteProcessing );
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
//		Processing &producerProc= GetProcessing(GetProcessingIdentifier(producer));
		OutPort & outport = GetOutPortByCompleteName(producer);
//		Processing * consumerProc = &GetProcessingIdentifier(consumer);
		InPort & inport = GetInPortByCompleteName(consumer);

		if (PortsAreConnected(outport, inport)) //currently are connected
		{
			return true;
		}

		inport.Attach(GetNode(outport));
		return true;
	}


	const char Network::NamesIdentifiersSeparator()
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

	
	NodeBase& Network::GetNode(OutPort & out)
	{
		
		if (out.GetNode() == 0) //if it hasn't associated node		 
		{
			//		BaseNode* node = new NodeTmpl<TIPUSDADES, CircularStreamImpl<TIPUSDADES>>;
//			_nodes.insert(node);
//			out.Attach(node);
		}
		return *(out.GetNode());
	}



}
