#ifndef __SDIFStream__
#define __SDIFStream__

#include "Type.hxx"
#include <list>

namespace SDIF
{

	class Frame;

	/** SDIF has a notion of streams: each frame contains a stream id.
	*       However, in the files the streams are not explicitely present,
	*       as the frames are ordered by time, not by stream. The 
	*       Stream is a list of Frames with the same
	*       id.
	*       
	*       @see Collection.
	*/
	class Stream
	{
	public:
		list<Frame*> mFrameList;
		TInt32 mStreamId;
	public:
		/** Create a Stream
		* @param streamId The StreamID for the frames in this stream
		*/
		Stream(TInt32 streamId);
		/** Add a Frame to the stream
		* @param pFrame The Frame that this stream starts with.
		*/
		void Add(Frame* pFrame);
		/** Parse all frames after the starting frame that have the same
		* stream ID into the streams list 
		*/
		//!!! void Parse(void);
	};

}
		
#endif
