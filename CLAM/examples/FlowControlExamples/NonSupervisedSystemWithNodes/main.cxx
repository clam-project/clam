#include "NonSupervisedSystemWithNodes.hxx"

int main()
{
	FlowControlExample::SystemWithNodes s(
		"testin.wav" /* file in */,
		"testout.wav" /* file out */, 
		512 /* frame size */, 
		500 /* frames to compute*/,
		true /* audio output*/);

	s.ProcessAllNetworkConfigurations();

	return 0;
}
