#ifndef _OutPortTmpl_hxx_
#define _OutPortTmpl_hxx_

#include "OutPort.hxx"
#include "Node.hxx"
#include "ProcessingData.hxx"
#include "WriteStreamRegion.hxx"
#include "Processing.hxx"

#include <string>

namespace CLAM
{

template<class T>
class OutPortTmpl : public OutPort
{
	WriteStreamRegion *mpRegion;
	Node<T> *mpNode;
	Array<T> mData;
public:
	inline OutPortTmpl(const std::string &n, Processing *o, int length, int hop = 0);
	inline T &GetData();
	inline void LeaveData();
	void Attach(ProcessingData& data);
	void Attach( NodeBase&);
	inline void Attach(T& data);
	inline void Attach(Node<T> &n);
	inline void Attach(OutPortTmpl<T> &p); // For composites
	inline void Accept(DataVisitor&);
	
	ProcessingData* GetProcessingData();
	NodeBase* GetNode();
	bool IsAttached();
	void Unattach();

};

// Implementation
//-------------------------------------------------------------------------------


template<class T>
OutPortTmpl<T>::OutPortTmpl(const std::string &n,
                            Processing *o,
                            int length,
                            int hop)
	: OutPort(n,o,length),
	  mpRegion(0),
	  mpNode(0)
{
	o->PublishOutPort(this);
}

template<class T>
void OutPortTmpl<T>::Attach(ProcessingData& data)
{
	try { 
		Attach(dynamic_cast<T&>(data));
	}
	catch (std::bad_cast){
		CLAM_ASSERT(false,"You are trying to attach a processing data that is not suitable for this port");
	}
}	

template<class T>
void OutPortTmpl<T>::Attach(T& data)
{
	mData.SetPtr(&data,1);
}	

template<class T>
void OutPortTmpl<T>::Attach(Node<T>& node)
{
	mData.SetPtr(0);
	mpNode = &node;
	mpRegion = node.NewWriter(this, Hop(), Length());
}

template<class T>
void OutPortTmpl<T>::Attach(OutPortTmpl<T>& port)
{
	if (port.mpNode)
		Attach(*port.mpNode);
	else
		Attach(*port.mpData);
}	

template<class T>
T &OutPortTmpl<T>::GetData()	
{ 
	CLAM_ASSERT(mData.Size() || (mpNode && mpRegion),
	            "OutPortTmpl::GetData(): No data atached to the port.");
	if (mpNode)
		mpNode->GetAndActivate(mpRegion,mData);
	return mData[0];
}

template<class T>
void OutPortTmpl<T>::Attach( NodeBase& node)
{
	try {
		Attach( dynamic_cast< Node<T>& >(node) );
	}
	catch (std::bad_cast) {
		CLAM_ASSERT(false,"You are trying to attach a node that is not suitable for this port");
	}
}

template<class T>
void OutPortTmpl<T>::LeaveData()	
{
	if (mpNode)
		mpNode->LeaveAndAdvance(mpRegion);
}

template<class T>
void OutPortTmpl<T>::Accept(DataVisitor& v)
{
	int i;
	for (i=0; i<mData.Size(); i++)
		v.Visit(mData[i]);
}

template<class T>
ProcessingData* OutPortTmpl<T>::GetProcessingData()
{
	if (IsAttached())
	{
		return &(mData[0]);
	}
	return 0;
}

template<class T>
bool OutPortTmpl<T>::IsAttached()
{
	return mData.Size()>0;
}

template<class T>
void OutPortTmpl<T>::Unattach()
{
	mData.SetPtr(NULL);
}

	
template<class T>
NodeBase* OutPortTmpl<T>::GetNode()
{
	return mpNode;
}



} // namespace CLAM

//\todo revise
// Port specializations
#include "AudioOutPortTmpl.hxx"

#endif
