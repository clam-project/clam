#ifndef _NullWaitMessage_hxx_
#define _NullWaitMessage_hxx_

namespace CLAMGUI
{
	class NullWaitMessage : public WaitMessage
	{
		public:
			NullWaitMessage() : WaitMessage("")
			{
			}
	};
}

#endif  
