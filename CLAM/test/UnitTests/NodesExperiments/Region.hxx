#ifndef Region_hxx
#define Region_hxx

#include <list>

class Region
{
public:
	
	typedef std::list<Region*> ReadingRegionsList;
	typedef ReadingRegionsList::iterator ReadingRegionsIterator;


	Region() : _pos(0), _size(1), _hop(1), _beginDistance(0)
	{
	}

	long& pos()
	{
		return _pos;
	}
	int& size()
	{
		return _size;
	}
	int& hop()
	{
		return _hop;
	}
	
	int& beginDistance()
	{
		return _beginDistance;
	}
	void beginDistance(int &dist)
	{
		_beginDistance = dist;		
	}
	void pos( const long & pos )
	{
		_pos = pos;
	}

	void size( const int & size )
	{
		_size = size;
		sizeChanged(size);
	}

	void hop( const int & hop )
	{
		CLAM_ASSERT( hop <= _size, "Region::hop() - hop can't be greater than size" );
		_hop = hop;
	}

	virtual Region* producerRegion() = 0;

	virtual ReadingRegionsIterator beginReaders() = 0;
	virtual ReadingRegionsIterator endReaders() = 0;

protected:
 	virtual void sizeChanged(const int & newSize) = 0;

private:
	long _pos;
	int _size;
	int _hop;
	int _beginDistance; /// offset from the beginning of the data structure 
};

#endif

