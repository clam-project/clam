#ifndef __ANALYSISSYNTHESISEXAMPLEGUI__
#define __ANALYSISSYNTHESISEXAMPLEGUI__

#include "UserInterface.hxx"
#include "WaitMessageGUI.hxx"
#include "ProgressGUI.hxx"
#include "AnalysisSynthesisExampleBase.hxx"
#include <pthread.h>
using namespace CLAM;

namespace CLAMGUI
{

class AnalysisSynthesisExampleGUI
	: public AnalysisSynthesisExampleBase
{
public:
	friend class UserInterface;

	void Run();

	Progress* CreateProgress(const char* title,float from,float to);

	WaitMessage* CreateWaitMessage(const char* title);

	static void* startfn_analyze( void* ptr );

	static void* startfn_synthesize( void* ptr );

	static void* startfn_LoadAnalysis(void* ptr);

	static void* startfn_StoreAnalysis(void* ptr);
	
	void AnalyzeThread();
	
	void SynthesizeThread();

	void LoadAnalysisThread(const std::string& filename);
	
	void StoreAnalysisThread();


protected:
	int mThreadState;
	pthread_t mThread;
	std::string mFilename;
};

}

#endif // AnalysisSynthesisExampleGUI.hxx
