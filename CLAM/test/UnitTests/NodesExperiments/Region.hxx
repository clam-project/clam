#ifndef Region_hxx
#define Region_hxx

#include <list>
#include "Assert.hxx"

class Region
{
public:
	
	typedef std::list<Region*> ReadingRegionsList;
	typedef ReadingRegionsList::iterator ReadingRegionsIterator;


	Region() : mPos(0), mSize(1), mHop(1), mBeginDistance(0)
	{
	}

	virtual ~Region()
	{
	}

	long& Pos()
	{
		return mPos;
	}
	int& Size()
	{
		return mSize;
	}
	int& Hop()
	{
		return mHop;
	}
	
	int& BeginDistance()
	{
		return mBeginDistance;
	}
	void BeginDistance(int &dist)
	{
		mBeginDistance = dist;		
	}
	void Pos( const long & pos )
	{
		mPos = pos;
	}

	void Size( const int & size )
	{
		mSize = size;
		SizeChanged(size);
	}

	void Hop( const int & hop )
	{
		CLAM_ASSERT( hop <= mSize, "Region::hop() - hop can't be greater than size" );
		mHop = hop;
	}

	virtual Region* ProducerRegion() = 0;
	virtual void RemoveProducer(){}

	virtual ReadingRegionsIterator BeginReaders() = 0;
	virtual ReadingRegionsIterator EndReaders() = 0;
	virtual void RemoveRegion( Region & Region ){}

protected:
 	virtual void SizeChanged(const int & newSize) = 0;

private:
	long mPos;
	int mSize;
	int mHop;
	int mBeginDistance; /// offset from the beginning of the data structure 
};

#endif

