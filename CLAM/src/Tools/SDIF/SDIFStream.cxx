#include "Stream.hxx"
#include "Frame.hxx"

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
