#include "NonSupervisedSystemWithPorts.hxx"

using FlowControlExample::System;

int main()
{
	System s(
		"snd.wav" /* file in */,
		"nonsupervisedsystemwithports.wav" /* file out */, 
		512 /* frame size */, 
		500 /* frames to compute*/ );

	s.ProcessAllIterations();


	return 0;
}
