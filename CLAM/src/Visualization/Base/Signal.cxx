#include "Signal.hxx"

namespace CLAMGUI
{
	Signal::tConnectionId        Signal::smLastConnectionId = 0;
	Signal::tConnectionIdStack   Signal::smFreeIdStack;

	Signal::tConnectionId Signal::AssignConnection()
	{
		if ( smFreeIdStack.empty() )
			return smLastConnectionId++;

		tConnectionId id = smFreeIdStack.top();
		smFreeIdStack.pop();
		return id;
	}

	void Signal::FreeConnectionId( Signal::tConnectionId freedConnectionId )
	{
		smFreeIdStack.push( freedConnectionId );
	}
}
