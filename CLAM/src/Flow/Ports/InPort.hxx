#ifndef InPort_hxx
#define InPort_hxx

#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"
#include <string>

namespace CLAM
{
	
class OutPortBase; 
class Processing;

class InPortBase
{
public:
	InPortBase( const std::string & name = "unnamed in port", Processing * proc = 0 );
	virtual ~InPortBase();
	OutPortBase * GetAttachedOutPort(); 
	const std::string & GetName();
	Processing * GetProcessing();
	virtual bool CanConsume()=0;
	virtual int GetSize()=0;
	virtual void SetSize(int newSize)=0;
	virtual int GetHop()=0;
	virtual void SetHop(int newHop)=0;
	void Disconnect();	

protected:
	OutPortBase * mAttachedOutPort;
	std::string mName;
	Processing * mProcessing;
};


template<typename Token>
class InPort : public InPortBase
{
	typedef WritingRegion<Token> ProperWritingRegion;
	typedef typename ProperWritingRegion::ProperReadingRegion ProperReadingRegion;

public:

	InPort( const std::string & name = "unnamed in port", Processing * proc = 0 )
		: InPortBase( name,proc )
	{
	}

	virtual ~InPort()
	{
		if(mAttachedOutPort)
			Disconnect();
	}

	const Token & GetData(int offset=0)
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

} // namespace CLAM

#endif
