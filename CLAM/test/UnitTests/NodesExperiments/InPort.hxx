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
	const Token& consumeData() const
	{
		return _region.accessStreamData();
	}

	void addReaderToWriterRegion( ProperWritingRegion& writer )
	{
		writer.linkRegions( _region );
	}

private:
	ProperReadingRegion _region;

};


#endif
