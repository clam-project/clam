
#include "ControlSender.hxx"
#include <iostream>
#include <cmath>


ControlSender::ControlSender(CLAM::TData freq, 
			     CLAM::TData samplingRate,
			     CLAM::TData phase)
	: _freq(freq),
	  _samplingRate(samplingRate),
	  _phase(phase),
	  _outControl1("control 1"),
	  _outControl2("control 2")
{
	_deltaPhase = (2*M_PI*freq)/_samplingRate;
	std::cout << "constructor ok" << std::endl;
}

bool ControlSender::Do()
{
	CLAM::TData newValue = sin(_phase);
	_phase += _deltaPhase;
	if (_phase > (2*M_PI))
	{
		_phase -= (2*M_PI);
	}
	CLAM::TData firstValue = std::abs(newValue);
	CLAM::TData secondValue = 1 - std::abs(newValue);

	_outControl1.SendControl(firstValue);
	_outControl2.SendControl(secondValue);
	       
	std::cout << newValue << " 1: " << firstValue << " 2: " << secondValue << std::endl;
	return true;
}

