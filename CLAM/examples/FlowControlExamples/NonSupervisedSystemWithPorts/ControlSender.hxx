
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

	CLAM::OutControl _outControl1;
	CLAM::OutControl _outControl2;

public:
	ControlSender(CLAM::TData freq = 20.0, 
		      CLAM::TData samplingRate = 44100,
		      CLAM::TData phase = 0.0);
	bool Do();
	virtual ~ControlSender(){}
};

















#endif
