#include "Thread.hxx"
#include "CBL.hxx"

namespace SALTO
{
	Application::Application( )
		: mRunning( false )
	{
	    mKernel = new Kernel;
		mUI = new UserInterface;

		mUI->SetStopHandle( makeMemberFunctor0( *this, Application, Stop ) );
	}
	
	Application::~Application()
	{
	}

	void Application::Start()
	{
		mRunning = true;
		mKernel->StartProcessing();
		mKernel->AttachToThread( mThread );
		mThread.Start();
		mUI->Run();
	}
	  
	void Application::Stop()
	{
		mUI->ExitTheLoop();
		if ( mRunning )
			{
				mThread.Stop();
				mKernel->StopProcessing();
			}
		mRunning = false;
	}
}
