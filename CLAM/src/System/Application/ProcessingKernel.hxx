#ifndef __PROCESSINGKERNEL__
#define __PROCESSINGKERNEL__

namespace CLAM
{

class Thread;

class ProcessingKernel
{

public:
	ProcessingKernel();

	virtual ~ProcessingKernel() {}

	virtual void ProcessingCleanup() = 0;

	virtual void ProcessingLoop() = 0;

	void AttachToThread( Thread& att_thread );

protected:

	bool LoopCondition();

	virtual bool UserCondition();

	void Cancel();

private:
	bool  mShouldRun;

};

}

#endif // ProcessingKernel.hxx
