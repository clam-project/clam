#ifndef OutPort_hxx
#define OutPort_hxx

#include "WritingRegion.hxx"
#include <list>
#include "InPort.hxx"

class OutPortBase
{
public:
	typedef std::list<InPortBase*> InPortsList;
	virtual void ConnectToIn(InPortBase& in) = 0;
	virtual void DisconnectFromIn(InPortBase & in) = 0;
	virtual bool IsConnectedTo(InPortBase & in) = 0;
	InPortsList::iterator BeginConnectedInPorts()
	{
		return mConnectedInPortsList.begin();
	}
	InPortsList::iterator EndConnectedInPorts()
	{
		return mConnectedInPortsList.end();
	}
	virtual ~OutPortBase(){}

protected:
	InPortsList mConnectedInPortsList;
};


template<typename Token>
class OutPort : public OutPortBase
{
	typedef OutPort<Token> ProperOutPort;
	typedef InPort<Token> ProperInPort;
	typedef WritingRegion<Token> ProperWritingRegion;


public:
	virtual ~OutPort()
	{
		InPortsList::iterator it;
		for( it=mConnectedInPortsList.begin(); it!=mConnectedInPortsList.end(); it++ )
			DisconnectFromIn(**it);
	}

	void ConnectToIn( InPortBase& in)
	{
		try
		{
			ConnectToConcreteIn( dynamic_cast<ProperInPort&>(in) );
		} catch (...) // could be std::bad_cast ?
		{
			CLAM_ASSERT( false,
				"OutPort<Token>::connectToIn coudn't connect to inPort "
				"because was not templatized by the same Token type as outPort" );
		}
	}


	void ConnectToConcreteIn(InPort<Token>& in)
	{
		CLAM_ASSERT( !in.GetAttachedOutPort(), "OutPort<Token>::ConnectToConcreteIn - Trying to connect an inport "
							    "already connected to another out port" );
		CLAM_ASSERT( !IsConnectedTo(in), "OutPort<Token>::ConnectToConcreteIn - Trying to connect an in port "
						"already connected to this out port" );
		mConnectedInPortsList.push_back(&in);
		in.AttachToOutPort(this, mRegion );
	}

	void DisconnectFromIn( InPortBase& in)
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

	void DisconnectFromConcreteIn(InPort<Token>& in)
	{
		CLAM_ASSERT( true == IsConnectedTo(in), "OutPort::DisconnectFromConcreteIn() - Trying to disconnect a "
							"non-connected region" );
		mConnectedInPortsList.remove(&in);
		in.UnAttach();
	}
	Token & operator[](int offset)
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
		
	void Produce()
	{
		mRegion.Produce();
	}

	bool CanProduce()
	{
		return mRegion.CanProduce();
	}
		
	bool IsConnectedTo(InPortBase & in)
	{
		InPortsList::iterator it;
		for( it=mConnectedInPortsList.begin(); it!=mConnectedInPortsList.end(); it++ )
			if(*it == &in) return true;
		return false;
	}
protected:
	ProperWritingRegion mRegion;
};

#endif

