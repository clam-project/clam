#ifndef __CONNECTION__
#define __CONNECTION__

namespace CLAMGUI
{

class Signal;

class Connection
{
public:
	typedef unsigned tConnectionId;

	Connection();
	Connection( tConnectionId id, Signal* connectedSignal );
	Connection& operator=( Connection& s );
	Connection( const Connection& s );

	tConnectionId GetID() const
	{
		return mID;
	}

	~Connection();

private:
	mutable bool  mMustFreeSignal;
	tConnectionId mID;
	Signal*       mConnectedSignal;
};

}

#endif // Connection.hxx
