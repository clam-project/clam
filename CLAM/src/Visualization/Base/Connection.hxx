#ifndef __CONNECTION__
#define __CONNECTION__

namespace CLAMGUI
{

class Signal;

class Connection
{
	typedef unsigned tConnectionId;
public:
	Connection();
	Connection( tConnectionId id, Signal* connectedSignal );
	Connection& operator=( Connection& s );
//	Connection( Connection& s );
	Connection( const Connection& s );

	tConnectionId GetID() const
	{
		return mID;
	}

	~Connection();

private:
	mutable bool          mMustFreeSignal;
	tConnectionId mID;
	Signal*       mConnectedSignal;
};

}

#endif // Connection.hxx
