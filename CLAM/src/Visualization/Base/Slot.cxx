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

		void Slot::Bind( Connection& conn )
		{
				mActiveConnections.push_back( conn );
		}

}
