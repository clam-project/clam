#include "NonSupervisedSystem.hxx"

using FlowControlExample::System;

int main()
{
	System s(
		"testin.wav" /* file in */,
		"testout.wav" /* file out */, 
		512 /* frame size */, 
		500 /* frames to compute*/ );

	s.ProcessAllIterations();

	return 0;
}
