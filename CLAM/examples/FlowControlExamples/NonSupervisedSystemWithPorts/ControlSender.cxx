
#include "ControlSender.hxx"
#include <iostream>
#include <cmath>


ControlSender::ControlSender(CLAM::TData freq, 
			     CLAM::TData samplingRate,
			     CLAM::TData phase,
			     int frameSize)
	: _outControl1("control 1"),
	  _outControl2("control 2")
{
	Configure(freq, samplingRate, phase, frameSize);
}

void ControlSender::Configure(CLAM::TData freq, CLAM::TData samplingRate, CLAM::TData phase, 
			      int frameSize)
{
	_freq = freq;
	_samplingRate = samplingRate;
	_phase = phase;
	_frameSize = frameSize;
	_deltaPhase = ((2*M_PI*_freq)/_samplingRate)* _frameSize;
}


bool ControlSender::Do()
{
	CLAM::TData newValue = sin(_phase);
	_phase += _deltaPhase;
	if (_phase > (2*M_PI))
	{
		_phase = fmod(_phase,2*M_PI);
	}

	CLAM::TData firstValue = std::abs(newValue);
	CLAM::TData secondValue = 1 - std::abs(newValue);

	_outControl1.SendControl(firstValue);
	_outControl2.SendControl(secondValue);
	       
	return true;
}

