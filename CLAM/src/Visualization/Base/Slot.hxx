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
		typedef std::list<Connection>        tConnectionList;
		typedef tConnectionList::iterator    tConnectionIterator;

public:

		void Bind(const Connection& conn );

		void Unbind();

		void Unbind( Connection::tConnectionId conn );

		unsigned  ActiveConnections() const
		{
			return mActiveConnections.size();
		}

		~Slot();

private:

		tConnectionList     mActiveConnections;
};

}

#endif // Slot.hxx
