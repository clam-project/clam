// SimpleNetwork

int main()
{
	FlowControlExample::SystemWithNetworks s(
		"testin.wav" /* file in */,
		"testout.wav" /* file out */, 
		512 /* frame size */, 
		500 /* frames to compute*/,
		false /* audio output*/);

	s.ProcessAllNetworks();

	return 0;
}
