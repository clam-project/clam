#include "ProgressGUI.hxx"
#include <pthread.h>
#include <iostream>
#ifndef WIN32
#include <unistd.h>
#define Sleep(var) usleep(var*1000)
#endif
#ifdef WIN32
#include <windows.h>
#endif


namespace CLAMGUI
{

ProgressGUI::ProgressGUI(const char* title,float from,float to)
	:Progress(title,from,to)
{
	if (flThread == pthread_self())
		{
			
			mFlProgress = new Fl_Progress;
			mFlProgress->mLabel->label(mTitle);
			mFlProgress->mSlider->range(mFrom,mTo);
			mFlProgress->mWindow->show();
		}else{
			requested = 0;
			
			requestedFrom = mFrom;
			requestedTo = mTo;
			requestedTitle = mTitle;
			while (requested==0)
				{
					Sleep(1);
				}
			
			mFlProgress = requested;
		}
}

ProgressGUI::~ProgressGUI()
{
	if (flThread == pthread_self())
		{
			
			delete mFlProgress;
		}else{
			delRequested = mFlProgress;
			while (delRequested)
				{
					Sleep(1);
				}
		}
}	

void ProgressGUI::Idle(void* ptr)
{
	std::cout << "Idle #1" << std::endl;
	if (requestedTitle)
		{
			
			const char* tmp = requestedTitle;
			float tmpFrom = requestedFrom;
			float tmpTo = requestedTo;
			requestedTitle = 0;
			Fl_Progress * o= new Fl_Progress;
			o->mLabel->label(tmp);
			o->mSlider->range(tmpFrom,tmpTo);
			o->mWindow->show();
			requested = o;
		}
	if (delRequested)
		{
			
			Fl_Progress* tmp = delRequested;
			delRequested = 0;
			delete tmp;
		}
}

Fl_Progress* ProgressGUI::requested = 0;
Fl_Progress* ProgressGUI::delRequested = 0;
const char* ProgressGUI::requestedTitle = 0;
float ProgressGUI::requestedFrom;
float ProgressGUI::requestedTo;
pthread_t ProgressGUI::flThread;

}
