
#ifndef __InPortPublisher_hxx__
#define __InPortPublisher_hxx__

#include "InPort.hxx"

namespace CLAM
{

template<typename Token>
class InPortPublisher : public InPortBase
{
	typedef InPort<Token> ProperInPort;
public:
	InPortPublisher( const std::string & name = "unnamed in port", Processing * proc = 0 )
		: InPortBase( name, proc ), mPublishedInPort(0)
	{
	}
	
	virtual ~InPortPublisher()
	{
	}

	// XR: BIG TODO: make this method const!
	/*const*/ Token & GetData(int offset=0)
	{
		CLAM_DEBUG_ASSERT( mPublishedInPort != 0, "InPortPublisher - no in port published" );
		mPublishedInPort->GetData( offset );
	}

	void PublishInPort( InPortBase & in )
	{
		try
		{
			ConcretePublishInPort( dynamic_cast<ProperInPort&>(in) );
		} catch (...) // could be std::bad_cast ?
		{
			CLAM_ASSERT( false,
			"InPortPublisher<Token>::PublishInPort coudn't connect to outPort "
			"because was not templatized by the same Token type as InPortPublisher" );
		}

	}

	void ConcretePublishInPort( ProperInPort & in )
	{
		mPublishedInPort = &in;
	}

	
	void SetSize( int newSize )
	{
		CLAM_DEBUG_ASSERT( mPublishedInPort != 0, "InPortPublisher - no in port published" );
		mPublishedInPort->SetSize( newSize );
	}
	
	int GetSize()
	{
		CLAM_DEBUG_ASSERT( mPublishedInPort != 0, "InPortPublisher - no in port published" );
		return mPublishedInPort->GetSize();
	}
		
	int GetHop()
	{
		CLAM_DEBUG_ASSERT( mPublishedInPort != 0, "InPortPublisher - no in port published" );
		return mPublishedInPort->GetHop();
	}
	
	void SetHop( int hop )
	{
		CLAM_DEBUG_ASSERT( mPublishedInPort != 0, "InPortPublisher - no in port published" );
		mPublishedInPort->SetHop( hop );
	}
	
	void Consume()
	{
		CLAM_DEBUG_ASSERT( mPublishedInPort != 0, "InPortPublisher - no in port published" );
		mPublishedInPort->Consume();
	}
	
	bool CanConsume()
	{
		CLAM_DEBUG_ASSERT( mPublishedInPort != 0, "InPortPublisher - no in port published" );
		return mPublishedInPort->CanConsume();
	}

	ProperInPort & GetPublishedInPort()
	{
		return *mPublishedInPort;
	}
			
 protected:

	ProperInPort * mPublishedInPort;
};

} // namespace CLAM

#endif // __InPortPublisher_hxx__

