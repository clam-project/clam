
#include "Region.hxx"
#include "Assert.hxx"

namespace CLAM
{

Region::Region() 
	: mPos(0), mSize(1), mHop(1), mBeginDistance(0)
{
}

Region::~Region()
{
}

long & Region::Pos()
{
	return mPos;
}

int & Region::Size()
{
	return mSize;
}

int & Region::Hop()
{
	return mHop;
}

int & Region::BeginDistance()
{
	return mBeginDistance;
}

void Region::BeginDistance(int &dist)
{
	mBeginDistance = dist;		
}

void Region::Pos( const long & pos )
{
	mPos = pos;
}

void Region::Size( const int & size )
{
	mSize = size;
	SizeChanged(size);
}

void Region::Hop( const int & hop )
{
	CLAM_ASSERT( hop <= mSize, "Region::hop() - hop can't be greater than size" );
	mHop = hop;
}

} // namespace CLAM

