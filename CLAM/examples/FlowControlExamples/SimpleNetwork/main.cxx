
#include "SupervisedSystemWithoutTrueFlowControl.hxx"

int main()
{
	FlowControlExample::SupervisedSystemWithoutTrueFlowControl s(
		"testin.wav" /* file in */,
		"testout.wav" /* file out */, 
		512 /* frame size */, 
		250 /* frames to compute*/,
		false /* audio output*/);

	s.ProcessAllNetworks();

	return 0;
}
