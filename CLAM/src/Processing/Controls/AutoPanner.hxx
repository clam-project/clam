
#ifndef _AutoPanner_hxx_
#define _AutoPanner_hxx_

#include "DataTypes.hxx"
#include "OutControl.hxx"

namespace CLAM
{
/**
 * \todo this class has to be a concrete Processing
 */
class AutoPanner
{
private:
	TData _freq;
	TData _samplingRate;
	TData _phase;
	TData _deltaPhase;
	int _frameSize;

public:
	OutControl _outControl1;
	OutControl _outControl2;

public:
	AutoPanner(TData freq,
		      TData samplingRate,
		      TData phase = 0.0,
		      int _frameSize = 1);

	void Configure(TData freq,
		       TData samplingRate,
		       TData phase,
		       int _frameSize);
	bool Do();
	virtual ~AutoPanner(){}
};

} //namespace CLAM

















#endif
