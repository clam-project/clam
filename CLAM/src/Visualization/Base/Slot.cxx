#include "Slot.hxx"
#include <algorithm>

namespace CLAMGUI
{

	class ConnectionSearchPred
	{
	public:

		ConnectionSearchPred( Connection::tConnectionId id )
			: mSoughtID( id )
		{
		}

		bool operator()( const Connection& conn )
		{
			return ( conn.GetID() == mSoughtID );
		}

	private:

		Connection::tConnectionId mSoughtID;
	};

	Slot::~Slot()
	{
		Unbind();
	}

	void Slot::Unbind()
	{
		mActiveConnections.clear();
	}

	void Slot::Bind( const Connection& conn )
	{
		mActiveConnections.push_back( conn );
	}

	void Slot::Unbind( Connection::tConnectionId conn )
	{
		tConnectionIterator i = std::find_if(	mActiveConnections.begin(), 
											mActiveConnections.end(), 
											ConnectionSearchPred( conn ) );

		mActiveConnections.erase( i );
	}

}
