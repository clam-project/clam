#ifndef __ANALYSISSYNTHESISEXAMPLEGUI__
#define __ANALYSISSYNTHESISEXAMPLEGUI__

#include "NotGeneratedUserInterface.hxx"
#include "WaitMessageGUI.hxx"
#include "ProgressGUI.hxx"
#include "AnalysisSynthesisExampleBase.hxx"
#include "Thread.hxx"
#include "SMS_DataExplorer.hxx"
#include "Slotv1.hxx"
using namespace CLAM;

class UserInterface;

namespace CLAMGUI
{

class AnalysisSynthesisExampleGUI
	: public CLAM::AnalysisSynthesisExampleBase
{
public:

	friend class UserInterface;

	AnalysisSynthesisExampleGUI();

	virtual ~AnalysisSynthesisExampleGUI();

	void Run();

	Progress* CreateProgress(const char* title,float from,float to);

	WaitMessage* CreateWaitMessage(const char* title);

	void SetCanvas( Fl_Smart_Tile* canvas );

	SigSlot::Slotv1<double>        TimeSelected;

protected: // methods

	void DoAnalysis();

	void DoSynthesis();

	void DoTracksCleanup();

	void DoLoadAnalysis();

	void DoStoreAnalysis();
	
	void DoTransformation();

	void OnNewTime( double time );

	void DoMorphAnalysis();
	void DoMorphTracksCleanup();

private:

	void ExecuteMethodOnThreadKeepingScreenUpToDate( CBL::Functor0 method );

protected:
	int                       mThreadState;
	std::string               mFilename;
	Thread                    mThread;
	CLAMVM::SMS_DataExplorer  mExplorer;
	UserInterface*            mUI;
};

}

#endif // AnalysisSynthesisExampleGUI.hxx
