#include "WaitMessageGUI.hxx"
#ifndef WIN32
#include <unistd.h>
#define Sleep(var) usleep(var*1000)
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
namespace CLAMGUI
{
	WaitMessageGUI::WaitMessageGUI(const char* title)
		:WaitMessage(title)
	{
		if (flThread == pthread_self())
			{
				
				mFlWaitMessage = new Fl_WaitMessage;
				mFlWaitMessage->mLabel->label(title);
				mFlWaitMessage->mWindow->show();
			}
		else
			{
				requested = 0;
				
				requestedTitle = title;
				while (requested==0)
					{
						Sleep(1);
					}
				
				mFlWaitMessage = requested;
			}
	}
	
	WaitMessageGUI::~WaitMessageGUI()
	{
		if (flThread == pthread_self())
		{
			delete mFlWaitMessage;
		}
		else
			{
				delRequested = mFlWaitMessage;
				while (delRequested)
					{
						Sleep(10);
					}
			}
	}	

	void WaitMessageGUI::Idle(void* ptr)
	{
		std::cout << "Idle #2" << std::endl;
		if (requestedTitle)
		{

			const char* tmp = requestedTitle;
			requestedTitle = 0;
			Fl_WaitMessage * o= new Fl_WaitMessage;
			o->mLabel->label(tmp);
			o->mWindow->show();
			requested = o;
		}
		if (delRequested)
		{

			Fl_WaitMessage* tmp = delRequested;
			delRequested = 0;
			delete tmp;
		}
	}



Fl_WaitMessage* WaitMessageGUI::requested = 0;
Fl_WaitMessage* WaitMessageGUI::delRequested = 0;
const char* WaitMessageGUI::requestedTitle = 0;
pthread_t WaitMessageGUI::flThread;


}
