#ifndef WritingRegion_hxx
#define WritingRegion_hxx

#include "ReadingRegion.hxx"
#include "StreamImpl.hxx"


//template<typename Token>
template< typename Token, template <class> class DataStructure = CLAM::PhantomBuffer>
class WritingRegion : public Region
{
public:
	typedef StreamImpl< Token, DataStructure > ProperStream;
	typedef Token ProperToken;
	typedef ReadingRegion< WritingRegion<Token,DataStructure> > ProperReadingRegion;

	WritingRegion()
	{
		SizeChanged( Size() );
	}
	virtual ~WritingRegion()
	{
	}

	Region::ReadingRegionsIterator BeginReaders()
	{
		return mReadingRegions.begin();
	}
	Region::ReadingRegionsIterator EndReaders()
	{
		return mReadingRegions.end();
	}

	void LinkRegions( ProperReadingRegion &  reader)
	{
		mReadingRegions.push_back(&reader);
		reader.LinkProducerRegion(*this);
		reader.LinkAndNotifySizeToStream( Stream() );
  	}

	void RemoveRegion( ProperReadingRegion & region )
	{
		mReadingRegions.remove( &region );
		region.RemoveProducer();
	}


	/**
		returns the underlying stream. Useful for testing.
	*/
	ProperStream& Stream()
	{
		return mStream;
	}

	/** when no linked reading region is found it returns the writing pos thus eviting overlap condition */
	int RearmostReadingPos()
	{
		int result = Pos();
		ReadingRegionsIterator it;
		for(it=mReadingRegions.begin(); it!=mReadingRegions.end(); it++)
			if( (*it)->Pos() < result) result = (*it)->Pos();

		return result;
	}
	/**
		Checks that after moving the writing-region no overlap occur with
		reading regions. This could happen since this is a circular buffer.
		Returns true if no overlapping occur so it can produce.
	*/
	bool CanProduce()
	{
		CLAM_DEBUG_ASSERT( FulfilsInvariant(), "WritingRegion::canProduce() - fulfils invariant checking failed" );
		return !mStream.ExistsCircularOverlap( RearmostReadingPos(), Pos()+Size() );
 	}

	void Produce()
	{
		CLAM_DEBUG_ASSERT( CanProduce(), "WritingRegion::produce() - WritingRegion can't produce" );
		Pos() += Hop();
		// reserve stream tokens for next position
		mStream.WriterHasAdvanced( *this );
	}


	Token& operator[](int offset)  // TODO decide if operator[0] is the best option to get the data chunk
	{

		CLAM_DEBUG_ASSERT(0 <= offset && offset < Size(), "WritingRegion::operator [] - Index out of bounds" ); 
		int physicalIndex = BeginDistance() + offset;

		if (offset==0)
			return mStream.Read(physicalIndex, Size());
		return mStream[physicalIndex];
	}

		
	bool FulfilsInvariant()
	{
		ReadingRegionsIterator it;
		for(it=mReadingRegions.begin(); it!=mReadingRegions.end(); it++)
			if( (*it)->Pos() >= Pos()+Size() )
				return false;
		return true;
	}

	int LogicalStreamSize()
	{
		return mStream.LogicalSize();
	}

	Region* ProducerRegion()
	{
		return 0;
	}
private:
	/** Don't allow copies. Thus copy contructor made private */
	WritingRegion(const WritingRegion<Token>& original)
	{
	}

	void SizeChanged(const int & newSize)
	{
		mStream.NewWritingRegionSize( *this );
	}

	ReadingRegionsList mReadingRegions;
	ProperStream  mStream;
};

#endif
