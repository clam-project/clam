#include "NonSupervisedSystemWithPorts.hxx"

using FlowControlExample::System;

int main()
{
	System s(
		"snd.wav" /* file in */,
		"nonsupervisedsystemwithports.wav" /* file out */, 
		512 /* frame size */, 
		5000 /* frames to compute*/ );

//	s.ProcessAllIterations();
	s.StablishConnections( &System::StablishConnectionsOscillatorToFileOut );
	s.DoProcessings( &System::OscillatorToFileOut );

	return 0;
}
