#include "Connection.hxx"
#include "Signal.hxx"

namespace CLAMGUI
{

	Connection::Connection()
		: mMustFreeSignal(false), mConnectedSignal( NULL )
	{
	}

	Connection::~Connection()
	{
		if ( mMustFreeSignal )
			mConnectedSignal->FreeConnection( this );
	}

	Connection::Connection( tConnectionId id, Signal* connSig )
		: mMustFreeSignal( true ), mID( id ), mConnectedSignal( connSig )
	{
	}

	Connection::Connection( const Connection& c )
	{
		mMustFreeSignal = c.mMustFreeSignal;
		mID = c.mID;
		mConnectedSignal = c.mConnectedSignal;
		c.mMustFreeSignal = false;
	}

	Connection& Connection::operator=( Connection& s )
	{
		mMustFreeSignal = s.mMustFreeSignal;
		mID = s.mID;
		mConnectedSignal = s.mConnectedSignal;
		s.mMustFreeSignal = false;

		return *this;
	}

}
