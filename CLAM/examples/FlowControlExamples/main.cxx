#include "NonSupervisedSystem.hxx"

using FlowControlExample::System;

int main()
{
	System s(
		"testin.wav" /* file in */,
		"testout.wav" /* file out */, 
		512 /* frame size */, 
		5000 /* iterations*/ );

	s.ProcessAllIterations();
//	s.DoProcessings( &System::OscillatorToFileOut );

	return 0;
}
