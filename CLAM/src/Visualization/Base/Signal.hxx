#ifndef __SIGNAL__
#define __SIGNAL__

#include <utility>
#include <map>
#include <stack>

namespace CLAMGUI
{

class Connection;

/**
 * A signal represents an event to be signaled to a group of Slots.
 *
 * This class is an abstract one, it only defines the common interface
 * for signals and some common implementation.
 */

class Signal
{
public:
	typedef unsigned  tConnectionId;

	virtual ~Signal(){}

	/**
	 * Undoes the given Conection
	 */
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
