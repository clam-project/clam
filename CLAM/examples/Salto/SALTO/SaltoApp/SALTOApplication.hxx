#ifndef __SALTO_APPLICATION__
#define __SALTO_APPLICATION__

#include "Thread.hxx"
using CLAM::Thread;
#include "SALTOKernel.hxx"
#include "SALTOGUI.hxx"

namespace SALTO
{

class Application
{
public:
	SALTOApplication();

	virtual ~SALTOApplication();

	void Start();
protected:
	
	void Stop();

private:
	bool             mRunning;
	bool             mToQuit;
	Kernel*          mKernel;
	UserInterface*   mUI;
	Thread           mThread;
};

}
#endif // SALTOApplication.hxx
