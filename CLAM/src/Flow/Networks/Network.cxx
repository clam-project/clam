
#include "Network.hxx"
#include <algorithm>

namespace CLAM
{
	namespace Helper
	{
		void DeleteSecond( Network::ProcessingsMap::value_type& mapElem ) {
			delete mapElem.second;
		}
	}
	// destructor
	Network::~Network()
	{
		std::for_each(_processings.begin(),	_processings.end(),	Helper::DeleteSecond );

	/*	ProcessingsMap::const_iterator it;
		for (it=_processings.begin(); it!=_processings.end(); it++ )
			delete it->second;
	*/

	}

	Processing& Network::GetProcessing( std::string & name )
	{
		CLAM_ASSERT( HasProcessing(name), "No Processing with the given name" );

		ProcessingsMap::const_iterator it = _processings.find( name );
		return *it->second;
	}

	void Network::AddProcessing( std::string & name, Processing* proc)
	{
		// returns false if the key was repeated.
		if (!_processings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );
	}

	bool Network::HasProcessing( std::string & name )
	{
		ProcessingsMap::const_iterator i = _processings.find( name );
		return i!=_processings.end();
	}

}
