#ifndef __SIGNAL__
#define __SIGNAL__

#include <utility>
#include <map>
#include <stack>

namespace CLAMGUI
{

class Connection;

class Signal
{
public:
	typedef unsigned  tConnectionId;

	virtual void FreeConnection( Connection* ) = 0;

protected:
	void FreeConnectionId( tConnectionId );
	tConnectionId AssignConnection();

	// Types
	typedef std::stack< unsigned >      tConnectionIdStack;
protected:
	
	static tConnectionIdStack  smFreeIdStack;
    static tConnectionId       smLastConnectionId;
};

}
#endif // Signal.hxx
