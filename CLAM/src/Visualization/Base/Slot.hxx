#ifndef __SLOT__
#define __SLOT__

#include "Connection.hxx"
#include <list>

namespace CLAMGUI
{

/**
 * An Slot is something you may connect with a signal.
 * @see Signal
 */
class Slot
{
		typedef std::list<Connection> tConnectionList;

public:

		void Bind(const Connection& conn );

		void Unbind();

		~Slot();

private:

		tConnectionList     mActiveConnections;
};

}

#endif // Slot.hxx
