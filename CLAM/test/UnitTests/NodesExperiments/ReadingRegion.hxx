#ifndef ReadingRegion_hxx
#define ReadingRegion_hxx

#include "Region.hxx"
#include "StreamImpl.hxx"

#include <vector>
#include <list>
#include <iostream> // TODO: push down to the cxx

template<class WritingRegion>
class ReadingRegion : public Region
{
	//typedef StreamImpl< Token > ProperStream;
	typedef typename WritingRegion::ProperStream Stream;
	typedef typename WritingRegion::ProperToken Token;

public:
	ReadingRegion() : _attachedStream(0), _producingRegion(0)
	{
	}

	void linkAndNotifySizeToStream(Stream& stream)
	{
		_attachedStream = &stream;
		_attachedStream->newReadingRegionSize( *this );
	}

	Stream& stream()
	{
		return *(_attachedStream);
	}

	/** Condition of overlap between reading and writing regions.
		Returns true if are not overlapping so it can consume
	*/
	bool canConsume()
	{
		assert(_producingRegion);
		return producerRegion()->pos() >= pos()+size();
	}
	/**
		This method must be called when the data of the reading-region has been
		already readen (consumed) and we want to advance the region position
		for further	readings (consumes)
	*/
	void consume()
	{
		assert( canConsume() ); // TODO: it should be a CLAM_DEBUG_ASSERT
		pos() += hop();
		_attachedStream->readerHasAdvanced( *this );
	}

	void linkProducerRegion( Region& writing)
	{
		_producingRegion = &writing;
		// it starts at the same position than the writer is in this exact moment
		pos( writing.pos() );
		beginDistance( writing.beginDistance() );
	}
	
	const Token& operator[](int offset)
	{
		assert( _attachedStream );
		assert( canConsume() ); // TODO should be CLAM_DEBUG_ASSERT: msg:region is still not completely produced

		int physicalIndex = beginDistance() + offset;

		assert( physicalIndex < _attachedStream->logicalSize());

		return _attachedStream->operator[](physicalIndex);
	}
	
	Region* producerRegion()
	{
		return _producingRegion;
	}
	
 	ReadingRegionsIterator beginReaders()
	{
		return 0; 
	}
	ReadingRegionsIterator endReaders()
	{
		return 0; 
	}
private:
	void sizeChanged(const int & newSize)
	{
		if (_attachedStream)
			_attachedStream->newReadingRegionSize(*this);
	}

	void dumpState() // TODO: implement and include std headers in the cxx
	{
		std::cout << "ReadingRegion pos="
		<< pos() << " size=" << size() << " hop: " << hop() << " anchorToStreamBuffer:"		<< anchorToStreamBuffer() << std::endl;
	}

	Stream* _attachedStream;
	Region* _producingRegion;

};

#endif
