#ifndef __PROGRESSGUI__
#define __PROGRESSGUI__

#include "Fl_Progress.hxx"
#include "Progress.hxx"
#include <pthread.h>

namespace CLAMGUI
{

class ProgressGUI
	: public Progress
{
public:
	Fl_Progress* mFlProgress;

	static Fl_Progress* requested;
	static Fl_Progress* delRequested;
	static const char* requestedTitle;
	static pthread_t flThread;
	static float requestedFrom;	
	static float requestedTo;	

	void Update(float val)
	{
		mFlProgress->mValue = val;
	}

	ProgressGUI( const char* title, float from, float to );

	~ProgressGUI();

	static void Idle( void* ptr );
};

}
#endif // ProgressGUI.hxx
