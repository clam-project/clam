#include "Slot.hxx"

namespace CLAMGUI
{

		Slot::~Slot()
		{
				Unbind();
		}

		void Slot::Unbind()
		{
				mActiveConnections.clear();
		}

		void Slot::Bind( const Connection& conn )
		{
				mActiveConnections.push_back( conn );
		}

}
