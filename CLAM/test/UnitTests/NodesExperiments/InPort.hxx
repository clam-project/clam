#ifndef InPort_hxx
#define InPort_hxx

#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"

class InPortBase
{
public:
	virtual ~InPortBase()
	{}
};


template<typename Token>
class InPort : public InPortBase
{
	typedef WritingRegion<Token> ProperWritingRegion;
	typedef typename ProperWritingRegion::ProperReadingRegion ProperReadingRegion;

public:

	// concrete interface
	const Token& ConsumeData() const
	{
		return mRegion.AccessStreamData();
	}

	void AddReaderToWriterRegion( ProperWritingRegion& writer )
	{
		writer.LinkRegions( mRegion );
	}

private:
	ProperReadingRegion mRegion;

};


#endif
