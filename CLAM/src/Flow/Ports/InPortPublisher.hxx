
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
	typedef std::list< ProperInPort * > ProperInPortsList;

	InPortPublisher( const std::string & name = "unnamed in port", Processing * proc = 0 )
		: InPortBase( name, proc )
	{
	}
	
	virtual ~InPortPublisher()
	{
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
		mPublishedInPortsList.push_back( &in );
	}
	
	int GetSize()
	{
		return 0;
	}
	
	void SetSize(int newSize)
	{
	}
	
	int GetHop()
	{
		return 0;
	}
	
	void SetHop(int newHop)
	{
	}
	
	bool CanConsume()
	{
		typename ProperInPortsList::iterator it;
		for(it=mPublishedInPortsList.begin(); it!=mPublishedInPortsList.end(); it++)
			if(!(*it)->CanConsume())
				return false;
		return true;	
	}
	

	typename ProperInPortsList::iterator BeginPublishedInPortsList()
	{
		return mPublishedInPortsList.begin();
	}
	
	typename ProperInPortsList::iterator EndPublishedInPortsList()
	{
		return mPublishedInPortsList.end();
	}
		
 protected:

	ProperInPortsList mPublishedInPortsList;
};

} // namespace CLAM

#endif // __InPortPublisher_hxx__

