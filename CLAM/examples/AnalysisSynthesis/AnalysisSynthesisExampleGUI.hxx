#ifndef __ANALYSISSYNTHESISEXAMPLEGUI__
#define __ANALYSISSYNTHESISEXAMPLEGUI__

#include "NotGeneratedUserInterface.hxx"
#include "WaitMessageGUI.hxx"
#include "ProgressGUI.hxx"
#include "AnalysisSynthesisExampleBase.hxx"
#include "Thread.hxx"
using namespace CLAM;

namespace CLAMGUI
{

class AnalysisSynthesisExampleGUI
	: public AnalysisSynthesisExampleBase
{
public:

	virtual ~AnalysisSynthesisExampleGUI()
	{
	}

	friend class UserInterface;

	void Run();

	CLAMGUI::Progress* CreateProgress(const char* title,float from,float to);

	CLAMGUI::WaitMessage* CreateWaitMessage(const char* title);


protected: // methods

	void DoAnalysis();

	void DoSynthesis();

	void DoTracksCleanup();

	void DoLoadAnalysis();

	void DoStoreAnalysis();

private:

	void ExecuteMethodOnThreadKeepingScreenUpToDate( CBL::Functor0 method );

protected:
	int mThreadState;
	std::string mFilename;
	Thread      mThread;
};

}

#endif // AnalysisSynthesisExampleGUI.hxx
