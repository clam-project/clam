#ifndef __SDIFCollection__
#define __SDIFCollection__

#include "DataTypes.hxx"
#include "SDIFStorage.hxx"

namespace SDIF
{
	class Frame;
	class Stream;

	/** A special Storage to store Frames in
	*	a linked list. 
	*
	* The Collection has the possibility to create the 
	*	Streams to which the frames belong, either on the fly, or on command
	*
	*	Typically, Collection is passed to File::Read, as the place
	*	the store the read Frames. @see File::Read
	*/
	class Collection:public Storage
	{
	public:
		typedef std::list<Frame*>::const_iterator iterator;

		//* The list of Frames
		list<Frame*> mFrameList;

		//* The list of Streams
		list<Stream*> mStreamList;

		//* Whether or not to create the Streams on the fly
		bool mAutoStreamAdding;
	public:
		//* Create a new Collection.
		Collection(bool autoStreamAdding = true);
		virtual ~Collection() {}

		void Add(Frame* pFrame);
		void Add(Stream* pStream);

		iterator Begin(void) const { return mFrameList.begin(); }
		iterator End(void) const { return mFrameList.end(); }

		Stream* FindStream(TInt32 streamId);

		void ParseStreams(void);
	};

}

#endif

