#ifndef ReadingRegion_hxx
#define ReadingRegion_hxx

#include "Region.hxx"
#include "StreamImpl.hxx"

#include <iostream> // TODO: push down to the cxx

namespace CLAM
{

template<class WritingRegion>
class ReadingRegion : public Region
{
	//typedef StreamImpl< Token > ProperStream;
	typedef typename WritingRegion::ProperStream ProperStream;
	typedef typename WritingRegion::ProperToken ProperToken;

public:
	ReadingRegion() : mAttachedStream(0), mProducingRegion(0)
	{
	}

	~ReadingRegion()
	{
		if(mProducingRegion)
			mProducingRegion->RemoveRegion( *this );
	}

	void LinkAndNotifySizeToStream( ProperStream& stream )
	{
		mAttachedStream = &stream;
		mAttachedStream->NewReadingRegionSize( *this );
	}

	ProperStream& Stream()
	{
		return *(mAttachedStream);
	}

	/** Condition of overlap between reading and writing regions.
		Returns true if are not overlapping so it can consume
	*/
	bool CanConsume()
	{
		if(!mProducingRegion)
			return false;
		return ProducerRegion()->Pos() >= Pos()+Size();
	}
	/**
		This method must be called when the data of the reading-region has been
		already readen (consumed) and we want to advance the region position
		for further	readings (consumes)
	*/
	void Consume()
	{
		CLAM_DEBUG_ASSERT( CanConsume(), "ReadingRegion::Consume() - region can't consume" );
		Pos() += Hop();
		mAttachedStream->ReaderHasAdvanced( *this );
	}

	void LinkProducerRegion( Region& writing)
	{
		mProducingRegion = &writing;
		// it starts at the same position than the writer is in this exact moment
		Pos( writing.Pos() );
		BeginDistance( writing.BeginDistance() );
	}
	
	/*
	 * const?? TODO!!
	 * */
	
	ProperToken& operator[](int offset)
	{
		CLAM_DEBUG_ASSERT( mAttachedStream, "ReadingRegion operator[] - No attached stream" );
		CLAM_DEBUG_ASSERT( CanConsume(), "ReadingRegion operator[] - region can't consume" );
		CLAM_DEBUG_ASSERT( 0 <= offset && offset < Size(), "ReadingRegion operator[] - Index out of bounds" );
		// TODO msg:region is still not completely produced

		int physicalIndex = BeginDistance() + offset;

		CLAM_ASSERT( physicalIndex < mAttachedStream->LogicalSize(), "ReadingRegion operator[] - Out of bounds access" );

		return mAttachedStream->operator[](physicalIndex);
	}
	
	Region* ProducerRegion()
	{
		return mProducingRegion;
	}

	void RemoveProducer()
	{
		mProducingRegion = 0;
		mAttachedStream = 0;
	}
	
 	ReadingRegionsIterator BeginReaders()
	{
		return 0; 
	}
	ReadingRegionsIterator EndReaders()
	{
		return 0; 
	}

private:
	void SizeChanged(const int & newSize)
	{
		if (mAttachedStream)
			mAttachedStream->NewReadingRegionSize(*this);
	}

	void DumpState() // TODO: implement and include std headers in the cxx
	{
		std::cout << "ReadingRegion pos="
		<< Pos() << " Size=" << Size() << " Hop: " << Hop() << " AnchorToStreamBuffer:"	<< AnchorToStreamBuffer() << std::endl;
	}

	ProperStream* mAttachedStream;
	
	Region* mProducingRegion;

};

} // namespace CLAM

#endif

