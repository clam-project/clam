#include "Signal.hxx"

namespace CLAMGUI
{
	Signal::tConnectionId        Signal::smLastConnectionId = 0;
	Signal::tConnectionIdStack   Signal::smFreeIdStack;

	Signal::tConnectionId Signal::AssignConnection()
	{
		tConnectionId id;
		if ( smFreeIdStack.empty() )
			{
				id = smLastConnectionId;
				smLastConnectionId++;
				return id;
			}
		else
			{
				id = smFreeIdStack.top();
				smFreeIdStack.pop();
				return id;
			}
	}

	void Signal::FreeConnectionId( Signal::tConnectionId freedConnectionId )
	{
		smFreeIdStack.push( freedConnectionId );
	}
}
