#ifndef __WAITMESSAGEGUI__
#define __WAITMESSAGEGUI__

#include "Fl_WaitMessage.hxx"
#include "WaitMessage.hxx"
#include <pthread.h>

namespace CLAMGUI
{

class WaitMessageGUI
	: public WaitMessage
{
public:

	Fl_WaitMessage* mFlWaitMessage;
	static Fl_WaitMessage* requested;
	static Fl_WaitMessage* delRequested;
	static const char* requestedTitle;
	static pthread_t flThread;
	
	
	WaitMessageGUI(const char* title);

	~WaitMessageGUI();

	static void Idle( void* ptr );

};

}

#endif // WaitMessageGUI.hxx
