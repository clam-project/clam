#ifndef __WritingRegion_hxx__
#define __WritingRegion_hxx__

#include "ReadingRegion.hxx"
#include "StreamImpl.hxx"

namespace CLAM
{

template< typename Token, template <class> class DataStructure = PhantomBuffer>
class WritingRegion : public Region
{
public:
	typedef StreamImpl< Token, DataStructure > ProperStream;
	typedef Token ProperToken;
	typedef ReadingRegion< WritingRegion<Token,DataStructure> > ProperReadingRegion;

	WritingRegion();
	virtual ~WritingRegion();

	Region::ReadingRegionsIterator BeginReaders();
	Region::ReadingRegionsIterator EndReaders();
	void LinkRegions( ProperReadingRegion &  reader);
	void RemoveRegion( Region & region );

	/**
		returns the underlying stream. Useful for testing.
	*/
	ProperStream& Stream();

	/** when no linked reading region is found it returns the writing pos thus eviting overlap condition */
	int RearmostReadingPos();
	
	/**
		Checks that after moving the writing-region no overlap occur with
		reading regions. This could happen since this is a circular buffer.
		Returns true if no overlapping occur so it can produce.
	*/
	bool CanProduce();
	void Produce();

	Token& operator[](int offset);  // TODO decide if operator[0] is the best option to get the data chunk	
	bool FulfilsInvariant();
	int LogicalStreamSize();
	Region* ProducerRegion();
private:
	/** Don't allow copies. Thus copy contructor made private */
	WritingRegion(const WritingRegion<Token>& original){}
	void SizeChanged(const int & newSize);
	
	ReadingRegionsList mReadingRegions;
	ProperStream  mStream;
};

/////// Implementation ////////

template< typename Token, template <class> class DataStructure>
WritingRegion< Token, DataStructure >::WritingRegion()
{
	SizeChanged( Size() );
}

template< typename Token, template <class> class DataStructure>
WritingRegion< Token, DataStructure >::~WritingRegion()
{	
	ReadingRegionsIterator it;
	for(it=BeginReaders(); it!=EndReaders(); it++)	
		(*it)->RemoveProducer();
	mReadingRegions.clear();
}

template< typename Token, template <class> class DataStructure>
Region::ReadingRegionsIterator WritingRegion< Token, DataStructure >::BeginReaders()
{
	return mReadingRegions.begin();
}

template< typename Token, template <class> class DataStructure>
Region::ReadingRegionsIterator WritingRegion< Token, DataStructure >::EndReaders()
{
	return mReadingRegions.end();
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::LinkRegions( ProperReadingRegion &  reader)
{
	mReadingRegions.push_back(&reader);
	reader.LinkProducerRegion(*this);
	reader.LinkAndNotifySizeToStream( Stream() );
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::RemoveRegion( Region & region )
{
	CLAM_ASSERT( region.ProducerRegion()==this, "Region::RemoveRegion() - Trying to remove an unlinked region");

	mReadingRegions.remove( &region );
	region.RemoveProducer();
}

template< typename Token, template <class> class DataStructure>
typename WritingRegion< Token, DataStructure>::ProperStream& WritingRegion< Token, DataStructure >::Stream()
{
	return mStream;
}

template< typename Token, template <class> class DataStructure>
int WritingRegion< Token, DataStructure >::RearmostReadingPos()
{
	int result = Pos();
	ReadingRegionsIterator it;
	for(it=mReadingRegions.begin(); it!=mReadingRegions.end(); it++)
		if( (*it)->Pos() < result) result = (*it)->Pos();

	return result;
}

template< typename Token, template <class> class DataStructure>
bool WritingRegion< Token, DataStructure >::CanProduce()
{
	CLAM_DEBUG_ASSERT( FulfilsInvariant(), "WritingRegion::canProduce() - fulfils invariant checking failed" );
	return !mStream.ExistsCircularOverlap( RearmostReadingPos(), Pos()+Size() );
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::Produce()
{
	CLAM_DEBUG_ASSERT( CanProduce(), "WritingRegion::produce() - WritingRegion can't produce" );
	Pos() += Hop();
	// reserve stream tokens for next position
	mStream.WriterHasAdvanced( *this );
}

template< typename Token, template <class> class DataStructure>
Token& WritingRegion< Token, DataStructure >::operator[](int offset) 
{

	CLAM_DEBUG_ASSERT(0 <= offset && offset < Size(), "WritingRegion::operator [] - Index out of bounds" ); 
	int physicalIndex = BeginDistance() + offset;

	if (offset==0)
		return mStream.Read(physicalIndex, Size());
	return mStream[physicalIndex];
}

template< typename Token, template <class> class DataStructure>
bool WritingRegion< Token, DataStructure >::FulfilsInvariant()
{
	ReadingRegionsIterator it;
	for(it=mReadingRegions.begin(); it!=mReadingRegions.end(); it++)
		if( (*it)->Pos() >= Pos()+Size() )
			return false;
	return true;
}

template< typename Token, template <class> class DataStructure>
int WritingRegion< Token, DataStructure >::LogicalStreamSize()
{
	return mStream.LogicalSize();
}

template< typename Token, template <class> class DataStructure>
Region* WritingRegion< Token, DataStructure >::ProducerRegion()
{
	return 0;
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::SizeChanged(const int & newSize)
{
	mStream.NewWritingRegionSize( *this );
}

} // namespace CLAM

#endif // __WritingRegion_hxx__

