
#ifndef StreamImpl_hxx
#define StreamImpl_hxx

#include "Region.hxx"
#include <list>
#include <vector>
#include "PhantomBuffer.hxx"
namespace CLAM
{

/** This declaration is necessary for VC7 compatibility */
template <typename T>
class StdList : public std::list<T>
{};

template <typename T>
class StdVector : public std::vector<T>
{};



template< typename Token, template <class> class DataStructure >
class StreamImpl
{

public:
	void NewWritingRegionSize( Region& writer  )
	{
		if ( writer.Size() <= LogicalSize() ) return;
		int newTokens = writer.Size() - LogicalSize();
		for( int i=0; i<newTokens; i++)
			mDataImpl.push_back(Token());
	}
private:
	void RegionHasAdvanced( Region& region ) 
	{
		region.BeginDistance() += region.Hop();

		if (region.BeginDistance() >= LogicalSize() ) // circular movement
			region.BeginDistance() -= LogicalSize();
	}
public:
	/** Do not check anything because the necessary checks are done at consume time*/
	void NewReadingRegionSize( Region& )
	{
	}

	void WriterHasAdvanced( Region& writer )
	{
		for( int i=0; i<writer.Hop(); i++)
			mDataImpl.push_back(Token());

		RegionHasAdvanced( writer );
	}

	void ReaderHasAdvanced( Region& reader )
	{
		// TODO: discard old tokens
		RegionHasAdvanced( reader);
	}
	/** This method is for generic interface convenience.
		It is used in the PhantomBuffer implementation.
	*/
	Token& Read(int physicalIndex, int size)
	{
		return operator[](physicalIndex);
	}

	Token& operator[](int physicalIndex)
	{
		CLAM_DEBUG_ASSERT( physicalIndex < int(mDataImpl.size()), "StreamImpl operator[] - Index out of bounds" );
		typename DataStructure<Token>::iterator it;
		int i;
		for(i=0, it = mDataImpl.begin();
			i<physicalIndex;
			it++, i++);
		return (*it);
	}

	int LogicalSize() const
	{
		return int(mDataImpl.size());
	}

	bool ExistsCircularOverlap(int rear, int writingHead) const
	{
		return false;
	}


private:
	DataStructure<Token> mDataImpl;
};



//---------------------------------------------------------------------------------




template< typename Token >
class StreamImpl<Token, PhantomBuffer>
{


public:
	
	void NewWritingRegionSize( Region& writer )
	{
		CLAM_DEBUG_ASSERT( writer.Size()>0, "StreamImpl::newWritingRegionSize() - size must be greater than 0" );
		CLAM_DEBUG_ASSERT( !writer.ProducerRegion(), "StreamImpl::newWritingRegionSize() - region must be a WritingRegion" );
		CommonNewRegionSize( writer );	
	}


	void NewReadingRegionSize( Region& reader )
	{
		CLAM_DEBUG_ASSERT( reader.ProducerRegion(), "StreamImpl::newReadingRegionSize() - region must be a ReadingRegion" );
		CommonNewRegionSize(reader);
	}
private:
	void CommonNewRegionSize( Region& anyRegion )
	{
		int logicalSizeCandidate = anyRegion.Size()*2;

		if(logicalSizeCandidate <= LogicalSize())
			return;
		
		Region & producer = anyRegion.ProducerRegion() ? (*anyRegion.ProducerRegion()) : anyRegion;
		int insertionPos = producer.BeginDistance();

		int newLogicalSize = 1 << ExponentOfClosestGreaterPowerOfTwo(logicalSizeCandidate);

		CLAM_DEBUG_ASSERT(newLogicalSize > LogicalSize(), "StreamImpl::commonNewRegionSize() - new logical size"
								"must be greater than the older logical size" ); 
		int tokensToInsert = newLogicalSize - LogicalSize();

		mDataImpl.Resize( 
				newLogicalSize, 
				anyRegion.Size()*2, // phantom buffer size
				insertionPos );
		
		UpdateBeginDistanceOfReadingRegions( producer, tokensToInsert );
	}

	
	bool ReaderAffectedByInsertion( Region & reader, Region & writer ) const
	{
		// a reader will be affected by the insertion of new tokens due a writer's resize if:
		
		// a) the reader is physically positioned (beginDistance) at the rear of the writer.
		if (reader.BeginDistance() > writer.BeginDistance()) 
			return true;
		
		// b) the reader is physically positioned (beginDistance) at the same position than the writer AND
		// is logically position (pos) after the writer. It means that the writer is exactly at
		// LogicalSize() positions before the reader.
		if( reader.BeginDistance()==writer.BeginDistance() && reader.Pos() < writer.Pos() )
			return true;
		return false;
	}
	
	void UpdateBeginDistanceOfReadingRegions( Region & writer, int tokensInserted )
	{
		/// traverses reading regions and only updates the ones
		/// that are at the right hand of the writer
		Region::ReadingRegionsIterator actualReader;

		for ( actualReader=writer.BeginReaders(); actualReader!=writer.EndReaders(); actualReader++)
			if( ReaderAffectedByInsertion(**actualReader, writer) )
				(*actualReader)->BeginDistance() += tokensInserted;
	}
	
	void RegionHasAdvanced( Region& region ) 
	{
		region.BeginDistance() += region.Hop();
		if (region.BeginDistance() >= LogicalSize() ) // circular movement
			region.BeginDistance() -= LogicalSize();
	}
public:
	/** Notifies the stream impl that the writing region have just produced */
	void WriterHasAdvanced( Region& writer )	
	{
		mDataImpl.Touch( writer.BeginDistance(), writer.Size() );
		RegionHasAdvanced( writer );
	}
	void ReaderHasAdvanced( Region& reader )
	{
		RegionHasAdvanced( reader );		
	}
		
	Token& Read(int physicalIndex, int size)
	{
		return *mDataImpl.Read( physicalIndex, size );
	}

	Token& operator[](int physicalIndex)
	{
		CLAM_DEBUG_ASSERT( physicalIndex < LogicalSize()+PhantomSize(), "StreamImpl::operator[] - Index out of bounds" );
		return Read( physicalIndex, 1);
	}

	int LogicalSize() const
	{
		return mDataImpl.LogicalSize();
	}

	/**
		This method appears only in the PhantomBuffer specialization template.
		Most useful for testing purposes.
	*/
	int PhantomSize()
	{
		return mDataImpl.PhantomSize();
	}

	bool ExistsCircularOverlap(int rear, int writingHead) const
	{
		return writingHead - rear > LogicalSize(); 
	}

private:
	int ExponentOfClosestGreaterPowerOfTwo( int newSize)
	{
		int newLogicalSize = 1;
		int power = 0;
		while( newLogicalSize < newSize )
		{
			newLogicalSize <<= 1;
			power++;
		}
		return power;
	}
	
	PhantomBuffer<Token> mDataImpl;
};

} // namespace CLAM

#endif // StreamImpl_hxx

