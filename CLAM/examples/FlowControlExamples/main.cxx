#include "NonSupervisedSystem.hxx"

int main()
{
	FlowControlExample::System s(
		"testout.wav" /* file out */, 
		512 /* frame size */, 
		5000 /* iterations*/ );
	
	s.ProcessAllIterations();

	return 0;
}