#ifndef OutPort_hxx
#define OutPort_hxx

#include "WritingRegion.hxx"


class OutPortBase
{
public:
	virtual void ConnectToIn(InPortBase& in) = 0;
	virtual InPortBase* ConnectedInPorts() const = 0;
	virtual ~OutPortBase()
	{}
};


template<typename Token>
class OutPort : public OutPortBase
{
	typedef OutPort<Token> ProperOutPort;
	typedef InPort<Token> ProperInPort;
	typedef WritingRegion<Token> ProperWritingRegion;


public:

	void ConnectToIn( InPortBase& in)
	{
		try
		{
			ConnectToConcreteIn( dynamic_cast<ProperInPort&>(in) );
		} catch (...) // could be std::bad_cast ?
		{
			CLAM_ASSERT( false,
				"OutPort<Token>::connectToIn coudn't connect to inPort"
				"because was not templatized by the same Token type as outPort" );
		}
	}


	void ConnectToConcreteIn(InPort<Token>& in)
	{
		mConnectedInPorts = &in;
		in.AddReaderToWriterRegion(mRegion);
	}

	InPort<Token>* ConnectedInPorts() const
	{
		return mConnectedInPorts;
	}

	// Concrete interface
	void ProduceData(const Token& token)
	{
		mRegion.AccessStreamData(token);
	}


private:
	InPort<Token>* mConnectedInPorts;


protected:
	ProperWritingRegion mRegion;
};

#endif

