#ifndef _InPortTmpl_hxx_
#define _InPortTmpl_hxx_

#include "InPort.hxx"
#include "Node.hxx"
#include "ProcessingData.hxx"
#include "ReadStreamRegion.hxx"
#include "Processing.hxx"

#include <string>


namespace CLAM
{


template<class T>
class InPortTmpl : public InPort
{
	ReadStreamRegion *mpRegion;
	Node<T> *mpNode;
	Array<T> mData;
public:
	inline InPortTmpl(const std::string &n, Processing *o, int length, int hop = 0, bool inplace=false);
	inline T &GetData();
	inline void LeaveData();
	//! implementation of the virtual method declared in InPort
	void Attach(ProcessingData& data);
	inline void Attach(T& data);
	//! implementation of the virtual method declared in InPort
	void Attach( NodeBase&);
	inline void Attach(Node<T> &n);
	inline void Attach(InPortTmpl<T> &p); // For composites
	inline void Accept(DataVisitor&);
	
	ProcessingData* GetProcessingData();
	NodeBase* GetNode();
	bool IsAttached();
	void Unattach();
};

//------------------------------------------------------------------------
// Template method implementations

template<class T>
InPortTmpl<T>::InPortTmpl(const std::string &n,
                          Processing *o,
                          int length,
                          int hop,
                          bool inplace)
	: InPort(n,o,length,hop,inplace),
	  mpRegion(0),
	  mpNode(0)
{
	o->PublishInPort(this);
}

template<class T>
T &InPortTmpl<T>::GetData()	
{ 
	CLAM_ASSERT(mData.Size() || (mpNode && mpRegion),
	            "InPortTmpl::GetData(): No data atached to the port.");
	if (mpNode)
		mpNode->GetAndActivate(mpRegion,mData);
	return mData[0];
}

template<class T>
void InPortTmpl<T>::LeaveData()	
{
	if (mpNode)
		mpNode->LeaveAndAdvance(mpRegion);
}

template<class T>
void InPortTmpl<T>::Attach(ProcessingData& data)
{
	try { 
		Attach(dynamic_cast<T&>(data));
	}
	catch (std::bad_cast){
		CLAM_ASSERT(false,"You are trying to attach a processing data that is not suitable for this port");
	}
}	

template<class T>
void InPortTmpl<T>::Attach( NodeBase& node)
{
	try {
		Attach( dynamic_cast< Node<T>& >(node) );
	}
	catch (std::bad_cast) {
		CLAM_ASSERT(false,"You are trying to attach a node that is not suitable for this port");
	}
}


template<class T>
void InPortTmpl<T>::Attach(T& data)
{
	mData.SetPtr(&data,1);
}	


template<class T>
void InPortTmpl<T>::Attach(Node<T>& node)
{
	mData.SetPtr(0);
	mpNode = &node;
	mpRegion = node.NewReader(this, Hop(), Length());
}	

template<class T>
void InPortTmpl<T>::Attach(InPortTmpl<T>& port)
{
	if (port.mpNode)
		Attach(*port.mpNode);
	else
		Attach(*port.mpData);
}	

template<class T>
void InPortTmpl<T>::Accept(DataVisitor& v)
{
	int i;
	for (i=0; i<mData.Size(); i++)
		v.Visit(mData[i]);
}

template<class T>
ProcessingData* InPortTmpl<T>::GetProcessingData()
{
	if (IsAttached())
	{
		return &(mData[0]);
	}
	return 0;
}

template<class T>
NodeBase* InPortTmpl<T>::GetNode()
{
	return mpNode;
}

template<class T>
bool InPortTmpl<T>::IsAttached()
{
	return mData.Size()>0;
}

template<class T>
void InPortTmpl<T>::Unattach()
{
	mpNode = 0;
	mData.SetPtr(NULL);
}
	

} // namespace CLAM

//\todo revise
// Port specializations
#include "AudioInPortTmpl.hxx"


#endif
