#ifndef InPort_hxx
#define InPort_hxx

#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"

class OutPortBase; 

class InPortBase
{
public:
	InPortBase();
	virtual ~InPortBase();
	OutPortBase * GetAttachedOutPort(); 

protected:
	// this method is called from the concrete class to call OutPort::DisconnectFromIn in .cxx, in order to
	// avoid double referencing of headers
	void DisconnectAndUnAttach();	
	
	OutPortBase * mAttachedOutPort;

};


template<typename Token>
class InPort : public InPortBase
{
	typedef WritingRegion<Token> ProperWritingRegion;
	typedef typename ProperWritingRegion::ProperReadingRegion ProperReadingRegion;

public:

	virtual ~InPort()
	{
		if(mAttachedOutPort)
			DisconnectAndUnAttach();
	}

	const Token & operator[](int offset)
	{
		return mRegion[offset];
	}
	
	void SetSize( int newSize )
	{
		mRegion.Size( newSize );
	}
	
	int GetSize()
	{
		return mRegion.Size();
	}

	int GetHop()
	{
		return mRegion.Hop();
	}
	
	void SetHop( int hop )
	{
		mRegion.Hop(hop);
	}
		
	void Consume() 
	{
		mRegion.Consume();
	}

	bool CanConsume()
	{
		return mRegion.CanConsume();
	}

	/**
	 *  This method is intended to be used only for the OutPort. A user shouldn't call it directly. 
	 *  Instead , use ConnectToIn method in OutPortBase.
	 */
	void AttachToOutPort( OutPortBase * out, ProperWritingRegion & writer )
	{
		writer.LinkRegions( mRegion );
		mAttachedOutPort = out;
	}
	/**
	 *  This method is intended to be used only for the OutPort. A user shouldn't call it directly. 
	 *  Instead , use DisconnectToIn method in OutPortBase.
	 */
	void UnAttach()
	{
		CLAM_DEBUG_ASSERT( mAttachedOutPort, "InPort<T>::UnAttach() - InPort is not connected" );
		mRegion.ProducerRegion()->RemoveRegion( mRegion );
		mAttachedOutPort = 0;
	}
protected:

	ProperReadingRegion mRegion;
};


#endif
