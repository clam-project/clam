#ifndef OutPort_hxx
#define OutPort_hxx

#include "WritingRegion.hxx"


class OutPortBase
{
public:
	virtual void connectToIn(InPortBase& in) = 0;
	virtual InPortBase* connectedInPorts() const = 0;
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

	void connectToIn( InPortBase& in)
	{
		try
		{
			connectToConcreteIn( dynamic_cast<ProperInPort&>(in) );
		} catch (...) // could be std::bad_cast ?
		{
			CLAM_ASSERT( false,
				"OutPort<Token>::connectToIn coudn't connect to inPort"
				"because was not templatized by the same Token type as outPort" );
		}
	}


	void connectToConcreteIn(InPort<Token>& in)
	{
		_connectedInPorts = &in;
		in.addReaderToWriterRegion(_region);
	}

	InPort<Token>* connectedInPorts() const
	{
		return _connectedInPorts;
	}

	// Concrete interface
	void produceData(const Token& token)
	{
		_region.accessStreamData(token);
	}


private:
	InPort<Token>* _connectedInPorts;


protected:
	ProperWritingRegion _region;
};

#endif

