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
		sizeChanged( size() );
	}
	virtual ~WritingRegion()
	{
	}

	Region::ReadingRegionsIterator beginReaders()
	{
		return _readingRegions.begin();
	}
	Region::ReadingRegionsIterator endReaders()
	{
		return _readingRegions.end();
	}

	void linkRegions( ProperReadingRegion &  reader)
	{
		_readingRegions.push_back(&reader);
		reader.linkProducerRegion(*this);
		reader.linkAndNotifySizeToStream( stream() );
  	}

	void removeRegion( ProperReadingRegion & region )
	{
		_readingRegions.remove( &region );
	}


	/**
		returns the underlying stream. Useful for testing.
	*/
	ProperStream& stream()
	{
		return _stream;
	}

	/** when no linked reading region is found it returns the writing pos thus eviting overlap condition */
	int rearmostReadingPos()
	{
		int result = pos();
		ReadingRegionsIterator it;
		for(it=_readingRegions.begin(); it!=_readingRegions.end(); it++)
			if( (*it)->pos() < result) result = (*it)->pos();

		return result;
	}
	/**
		Checks that after moving the writing-region no overlap occur with
		reading regions. This could happen since this is a circular buffer.
		Returns true if no overlapping occur so it can produce.
	*/
	bool canProduce()
	{
		CLAM_DEBUG_ASSERT( fulfilsInvariant(), "WritingRegion::canProduce() - fulfils invariant checking failed" );
		return !_stream.existsCircularOverlap( rearmostReadingPos(), pos()+size() );
 	}

	void produce()
	{
		CLAM_DEBUG_ASSERT( canProduce(), "WritingRegion::produce() - WritingRegion can't produce" );
		pos() += hop();
		// reserve stream tokens for next position
		_stream.writerHasAdvanced( *this );
	}


	Token& operator[](int offset)  // TODO decide if operator[0] is the best option to get the data chunk
	{

		CLAM_DEBUG_ASSERT(0 <= offset && offset < size(), "WritingRegion::operator [] - Index out of bounds" ); 
		int physicalIndex = beginDistance() + offset;

		if (offset==0)
			return _stream.read(physicalIndex, size());
		return _stream[physicalIndex];
	}

		
	bool fulfilsInvariant()
	{
		ReadingRegionsIterator it;
		for(it=_readingRegions.begin(); it!=_readingRegions.end(); it++)
			if( (*it)->pos() >= pos()+size() )
				return false;
		return true;
	}

	int logicalStreamSize()
	{
		return _stream.logicalSize();
	}

	Region* producerRegion()
	{
		return 0;
	}
private:
	/** Don't allow copies. Thus copy contructor made private */
	WritingRegion(const WritingRegion<Token>& original)
	{
	}

	void sizeChanged(const int & newSize)
	{
		_stream.newWritingRegionSize( *this );
	}

	ReadingRegionsList _readingRegions;
	ProperStream  _stream;
};

#endif
