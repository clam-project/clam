
#ifndef __CONTROL_SENDER_HXX__
#define __CONTROL_SENDER_HXX__

#include "DataTypes.hxx"
#include "OutControl.hxx"

class ControlSender
{
private:
	CLAM::TData _freq;
	CLAM::TData _samplingRate;
	CLAM::TData _phase;
	CLAM::TData _deltaPhase;
	int _frameSize;

public:
	CLAM::OutControl _outControl1;
	CLAM::OutControl _outControl2;

public:
	ControlSender(CLAM::TData freq,
		      CLAM::TData samplingRate,
		      CLAM::TData phase = 0.0,
		      int _frameSize = 1);

	void Configure(CLAM::TData freq,
		       CLAM::TData samplingRate,
		       CLAM::TData phase,
		       int _frameSize);
	bool Do();
	virtual ~ControlSender(){}
};

















#endif
