
#ifndef StreamImpl_hxx
#define StreamImpl_hxx

#include "Region.hxx"
#include <list>
#include <vector>

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
	void newWritingRegionSize( Region& writer  )
	{
		if ( writer.size() <= logicalSize() ) return;
		int newTokens = writer.size() - logicalSize();
		for( int i=0; i<newTokens; i++)
			_dataImpl.push_back(Token());
	}
private:
	void regionHasAdvanced( Region& region ) 
	{
		region.beginDistance() += region.hop();

		if (region.beginDistance() >= logicalSize() ) // circular movement
			region.beginDistance() -= logicalSize();
	}
public:
	/** Do not check anything because the necessary checks are done at consume time*/
	void newReadingRegionSize( Region& )
	{
	}

	void writerHasAdvanced( Region& writer )
	{
		for( int i=0; i<writer.hop(); i++)
			_dataImpl.push_back(Token());

		regionHasAdvanced( writer );
	}

	void readerHasAdvanced( Region& reader )
	{
		// TODO: discard old tokens
		regionHasAdvanced( reader);
	}
	/** This method is for generic interface convenience.
		It is used in the PhantomBuffer implementation.
	*/
	Token& read(int physicalIndex, int size)
	{
		return operator[](physicalIndex);
	}

	Token& operator[](int physicalIndex)
	{
		CLAM_DEBUG_ASSERT( physicalIndex < int(_dataImpl.size()), "StreamImpl operator[] - Index out of bounds" );
		typename DataStructure<Token>::iterator it;
		int i;
		for(i=0, it = _dataImpl.begin();
			i<physicalIndex;
			it++, i++);
		return (*it);
	}

	int logicalSize() const
	{
		return int(_dataImpl.size());
	}

	bool existsCircularOverlap(int rear, int writingHead) const
	{
		return false;
	}


private:
	DataStructure<Token> _dataImpl;
};



//---------------------------------------------------------------------------------


#include "PhantomBuffer.hxx"

template< typename Token >
class StreamImpl<Token, CLAM::PhantomBuffer>
{


public:
	
	void newWritingRegionSize( Region& writer )
	{
		CLAM_DEBUG_ASSERT( writer.size()>0, "StreamImpl::newWritingRegionSize() - size must be greather than 0" );
		CLAM_DEBUG_ASSERT( !writer.producerRegion(), "StreamImpl::newWritingRegionSize() - region must be a WritingRegion" );
		commonNewRegionSize( writer );	
	}


	void newReadingRegionSize( Region& reader )
	{
		CLAM_DEBUG_ASSERT( reader.producerRegion(), "StreamImpl::newReadingRegionSize() - region must be a ReadingRegion" );
		commonNewRegionSize(reader);
	}
private:
	void commonNewRegionSize( Region& anyRegion )
	{
		int logicalSizeCandidate = anyRegion.size()*2;

		if(logicalSizeCandidate <= logicalSize())
			return;
		
		Region & producer = anyRegion.producerRegion() ? (*anyRegion.producerRegion()) : anyRegion;
		int insertionPos = producer.beginDistance();

		int newLogicalSize = 1 << exponentOfClosestGreaterPowerOfTwo(logicalSizeCandidate);

		CLAM_DEBUG_ASSERT(newLogicalSize > logicalSize(), "StreamImpl::commonNewRegionSize() - new logical size"
								"must be greater than the older logical size" ); 
		int tokensToInsert = newLogicalSize - logicalSize();

		_dataImpl.Resize( 
				newLogicalSize, 
				anyRegion.size()*2, // phantom buffer size
				insertionPos );
		
		updateBeginDistanceOfReadingRegions( producer, tokensToInsert );
	}

	
	bool readerAffectedByInsertion( Region & reader, Region & writer ) const
	{
		// a reader will be affected by the insertion of new tokens due a writer's resize if:
		
		// a) the reader is physically positioned (beginDistance) at the rear of the writer.
		if (reader.beginDistance() > writer.beginDistance()) 
			return true;
		
		// b) the reader is physically positioned (beginDistance) at the same position than the writer AND
		// is logically position (pos) after the writer. It means that the writer is exactly at
		// logicalSize() positions before the reader.
		if( reader.beginDistance()==writer.beginDistance() && reader.pos() < writer.pos() )
			return true;
		return false;
	}
	
	void updateBeginDistanceOfReadingRegions( Region & writer, int tokensInserted )
	{
		/// traverses reading regions and only updates the ones
		/// that are at the right hand of the writer
		Region::ReadingRegionsIterator actualReader;

		for ( actualReader=writer.beginReaders(); actualReader!=writer.endReaders(); actualReader++)
			if( readerAffectedByInsertion(**actualReader, writer) )
				(*actualReader)->beginDistance() += tokensInserted;
	}
	
	void regionHasAdvanced( Region& region ) 
	{
		region.beginDistance() += region.hop();
		if (region.beginDistance() >= logicalSize() ) // circular movement
			region.beginDistance() -= logicalSize();
	}
public:
	/** Notifies the stream impl that the writing region have just produced */
	void writerHasAdvanced( Region& writer )	
	{
		_dataImpl.Touch( writer.beginDistance(), writer.size() );
		regionHasAdvanced( writer );
	}
	void readerHasAdvanced( Region& reader )
	{
		regionHasAdvanced( reader );		
	}
		
	Token& read(int physicalIndex, int size)
	{
		return *_dataImpl.Read( physicalIndex, size );
	}

	Token& operator[](int physicalIndex)
	{
		CLAM_DEBUG_ASSERT( physicalIndex < logicalSize()+phantomSize(), "StreamImpl::operator[] - Index out of bounds" );
		return read( physicalIndex, 1);
	}

	int logicalSize() const
	{
		return _dataImpl.LogicalSize();
	}

	/**
		This method appears only in the PhantomBuffer specialization template.
		Most useful for testing purposes.
	*/
	int phantomSize()
	{
		return _dataImpl.PhantomSize();
	}

	bool existsCircularOverlap(int rear, int writingHead) const
	{
		return writingHead - rear > logicalSize(); 
	}

private:
	int exponentOfClosestGreaterPowerOfTwo( int newSize)
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
	
	CLAM::PhantomBuffer<Token> _dataImpl;
};




#endif // StreamImpl_hxx
