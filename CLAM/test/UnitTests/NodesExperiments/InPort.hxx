#ifndef InPort_hxx
#define InPort_hxx

#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"


class OutPortBase; // TODO: include?

class InPortBase
{
public:
	InPortBase() : mAttachedOutPort(0){}
	virtual ~InPortBase()
	{}
	OutPortBase * GetAttachedOutPort() 
	{
		return mAttachedOutPort;
	}

	/**
	 *  This method is intended to be used only for the OutPort. A user shouldn't call it directly. 
	 *  Instead , use ConnectToIn method in OutPortBase.
	 */
	void AttachToOutPort( OutPortBase * out )
	{
		mAttachedOutPort = out;
	}
	
	/**
	 *  This method is intended to be used only for the OutPort. A user shouldn't call it directly. 
	 *  Instead , use DisconnectToIn method in OutPortBase.
	 */
	void UnAttach()
	{
		mAttachedOutPort = 0;
	}
protected:
	OutPortBase * mAttachedOutPort;
	
};


template<typename Token>
class InPort : public InPortBase
{
	typedef WritingRegion<Token> ProperWritingRegion;
	typedef typename ProperWritingRegion::ProperReadingRegion ProperReadingRegion;

public:
	ProperReadingRegion & GetRegion()
	{
		return mRegion;
	}

private:
	ProperReadingRegion mRegion;
};


#endif
