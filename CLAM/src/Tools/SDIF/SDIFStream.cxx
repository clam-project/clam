#include "SDIFStream.hxx"
#include "SDIFFrame.hxx"

namespace SDIF
{

	Stream::Stream(TInt32 streamId)
	:mStreamId(streamId)
	{
	}

	void Stream::Add(Frame* pFrame)
	{
		mFrameList.push_back(pFrame);
	}

}
