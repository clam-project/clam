#include "Thread.hxx"
#include "CBL.hxx"

namespace SALTO
{
	Application::Application( )
		: mRunning( false )
	{
		mUI->SetStopHandle( makeMemberFunctor0( *this, Application, Stop ) );
	}
	
	Application::~Application()
	{
	}

	void Application::Start()
	{
		mRunning = true;
		mKernel.StartProcessing();
		mKernel.AttachToThread( mThread );
		mThread.Start();
		mUI.Run();
	}
	  
	void Application::Stop()
	{
		mUI.ExitTheLoop();
		if ( mRunning ) {
			mThread.Stop();
		}
		mRunning = false;
	}
}
