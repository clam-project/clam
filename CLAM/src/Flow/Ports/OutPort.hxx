#ifndef __OutPort_hxx__
#define __OutPort_hxx__

#include "WritingRegion.hxx"
#include <list>
#include <string>
#include "InPort.hxx"
#include "InPortPublisher.hxx"

namespace CLAM
{

class Processing;

class OutPortBase
{
public:
	typedef std::list<InPortBase*> InPortsList;
	OutPortBase( const std::string & name = "unnamed in port", Processing * proc = 0 );
	virtual ~OutPortBase();
	const std::string & GetName();
	Processing * GetProcessing();
	InPortsList::iterator BeginConnectedInPorts();
	InPortsList::iterator EndConnectedInPorts();
	
	virtual void ConnectToIn(InPortBase& in) = 0;
	virtual void DisconnectFromIn(InPortBase & in) = 0;
	virtual void DisconnectFromAll()=0;
	virtual bool IsConnectedTo(InPortBase & in) = 0;
	virtual bool IsConnectableTo(InPortBase & ) = 0;
	virtual bool CanProduce()=0;
	virtual int GetSize()=0;
	virtual void SetSize(int newSize)=0;
	virtual int GetHop()=0;
	virtual void SetHop(int newHop)=0;
	bool HasConnections(){return mConnectedInPortsList.size();}
	virtual void CenterEvenRegions()=0;

protected:
	
	InPortsList mConnectedInPortsList;	
	std::string mName;
	Processing * mProcessing;
};


template<typename Token>
class OutPort : public OutPortBase
{
	typedef OutPort<Token> ProperOutPort;
	typedef InPort<Token> ProperInPort;
	typedef WritingRegion<Token> ProperWritingRegion;


public:
	OutPort( const std::string & name = "unnamed in port", Processing * proc = 0 );
	virtual ~OutPort();

	void DisconnectFromAll();
	void ConnectToIn( InPortBase& in);
	void ConnectToConcreteIn(InPort<Token>& in);
	void DisconnectFromIn( InPortBase& in);
	void DisconnectFromConcreteIn(InPort<Token>& in);
	bool IsConnectableTo(InPortBase & in);
	bool IsConnectedTo(InPortBase & in);
	
	Token & GetData(int offset=0);	
	void SetSize( int newSize );
	int GetSize();
	int GetHop();
	void SetHop( int hop );	
	void Produce();
	bool CanProduce();
	void CenterEvenRegions();

protected:	
	// XR TODO: TryDisconnect
	bool TryConnectToPublisher( InPortBase & in );
	bool TryConnectToConcreteIn( InPortBase & in );

	ProperWritingRegion mRegion;
};

template<class Token>
OutPort<Token>::OutPort( const std::string & name, Processing * proc )
	: OutPortBase(name,proc)
{
}

template<class Token>
void OutPort<Token>::DisconnectFromAll()
{
	InPortsList::iterator it = mConnectedInPortsList.begin();
	for( it=BeginConnectedInPorts(); it!=EndConnectedInPorts(); it++ )	
	{ 
		ProperInPort & in = dynamic_cast<ProperInPort&>(**it);
		in.UnAttach();
	}
	mConnectedInPortsList.clear();
}

template<class Token>
OutPort<Token>::~OutPort()
{
	InPortsList::iterator it = mConnectedInPortsList.begin();
	for( it=BeginConnectedInPorts(); it!=EndConnectedInPorts(); it++ )
	{ 
		ProperInPort & in = dynamic_cast<ProperInPort&>(**it);
		in.UnAttach();
	}
	mConnectedInPortsList.clear();
}

template<class Token>
bool OutPort<Token>::TryConnectToConcreteIn( InPortBase & in )
{
	try
	{
		ConnectToConcreteIn( dynamic_cast<ProperInPort&>(in) );
	}
	catch(...)
	{
		return false;
	}
	return true;
}

template<class Token>
bool OutPort<Token>::TryConnectToPublisher( InPortBase & in )
{
	try
	{
		InPortPublisher<Token> & publisher =  dynamic_cast< InPortPublisher<Token> &>(in);
		typename InPortPublisher<Token>::ProperInPortsList::iterator it;

		for( it=publisher.BeginPublishedInPortsList(); it!=publisher.EndPublishedInPortsList(); it++)
		{
			ConnectToConcreteIn( **it );
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

template<class Token>
void OutPort<Token>::ConnectToIn( InPortBase& in)
{
	CLAM_ASSERT( TryConnectToConcreteIn( in ) || TryConnectToPublisher( in ),
		     "OutPort<Token>::connectToIn coudn't connect to inPort "
   		     "because was not templatized by the same Token type as outPort" );
}

template<class Token>
void OutPort<Token>::ConnectToConcreteIn(InPort<Token>& in)
{
	CLAM_ASSERT( !in.GetAttachedOutPort(), "OutPort<Token>::ConnectToConcreteIn - Trying to connect an inport "
						    "already connected to another out port" );
	CLAM_ASSERT( !IsConnectedTo(in), "OutPort<Token>::ConnectToConcreteIn - Trying to connect an in port "
					"already connected to this out port" );
	mConnectedInPortsList.push_back(&in);
	in.AttachToOutPort(this, mRegion );
}

template<class Token>
void OutPort<Token>::DisconnectFromIn( InPortBase& in)
{
	try{
		DisconnectFromConcreteIn( dynamic_cast<ProperInPort&>(in) );
	} catch (...) // could be std::bad_cast ?
	{
		CLAM_ASSERT( false,
			"OutPort<Token>::DisConnectFromIn coudn't disconnect from inPort"
			"because was not templatized by the same Token type as outPort" );
	}
}

template<class Token>
void OutPort<Token>::DisconnectFromConcreteIn(InPort<Token>& in)
{
	CLAM_ASSERT( true == IsConnectedTo(in), "OutPort::DisconnectFromConcreteIn() - Trying to disconnect a "
						"non-connected region" );
	mConnectedInPortsList.remove(&in);
	in.UnAttach();
}

template<class Token>
Token & OutPort<Token>::GetData(int offset )
{
	return mRegion[offset];
}

template<class Token>
void OutPort<Token>::SetSize( int newSize )
{
	mRegion.Size( newSize );
}

template<class Token>
int OutPort<Token>::GetSize()
{
	return mRegion.Size();
}

template<class Token>
int OutPort<Token>::GetHop()
{
	return mRegion.Hop();
}

template<class Token>
void OutPort<Token>::SetHop( int hop )
{
	mRegion.Hop(hop);
}

template<class Token>
void OutPort<Token>::Produce()
{
	mRegion.Produce();
}

template<class Token>
bool OutPort<Token>::CanProduce()
{
	return mRegion.CanProduce();
}

template<class Token>
bool OutPort<Token>::IsConnectableTo(InPortBase & in)
{	
	return ((dynamic_cast< ProperInPort* >(&in)) != 0);
}

template<class Token>
bool OutPort<Token>::IsConnectedTo(InPortBase & in)
{
	InPortsList::iterator it;
	for( it=mConnectedInPortsList.begin(); it!=mConnectedInPortsList.end(); it++ )
		if(*it == &in) return true;
	return false;
}
	
template<class Token>
void OutPort<Token>::CenterEvenRegions()
{
	mRegion.CenterEvenRegions();
}

} // namespace CLAM

#endif // __OutPort_hxx__

