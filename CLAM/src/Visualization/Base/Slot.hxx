#ifndef __SLOT__
#define __SLOT__

#include "Connection.hxx"
#include <list>

namespace CLAMGUI
{

class Slot
{
		typedef std::list<Connection> tConnectionList;

public:

		void Bind( Connection& conn );

		void Unbind();

		~Slot();

private:

		tConnectionList     mActiveConnections;
};

}

#endif // Slot.hxx
