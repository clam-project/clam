#ifndef __SIGNALV1IMPLSERIOUS__
#define __SIGNALV1IMPLSERIOUS__

#ifndef __SIGNALV1__
#error "This is an internal implementation header. You are not allowed to include it directly!"
#endif

#include "ConnectionHandler.hxx"

namespace CLAMGUI
{

template < typename ParmType1 >
	class Signalv1 : public Signal
{
public:
	typedef typename CBL::Functor1<ParmType1>                    tCallbackType;

public:
	
	template < class RefType, typename PtrMember >
		Slot Connect( RefType thisRef, PtrMember pMember )
	{
		Slot s( AssignSlot(), this );

		mSuper.AddCallback( s.GetID(), CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, *thisRef, pMember ) );

		return s;
	}

	template < typename PtrFunction >
		Slot Connect( PtrFunction pMember )
	{
		Slot s( AssignSlot(), this );

		mSuper.AddCallback( s.GetID(), CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, pMember ) );

		return s;
	}
	
	void Emit( ParmType1 parm )
	{
		if ( mSuper.HasNoCallbacks() )
			return;
		
		tSuperType::tCallList calls = mSuper.GetCalls();
		tSuperType::tCallIterator i = calls.begin();
		tSuperType::tCallIterator end = calls.end();

		while ( i != end )
			{
				(*(*i))( parm );
				i++;
			}
		
	}

	void FreeSlot( Slot* pSlot )
	{
		mSuper.RemoveCall( pSlot->GetID() );
		FreeSlotId( pSlot->GetID() );
	}
private:
	typedef Signalv1<ParmType1>                          tSignalType;
	typedef typename ConnectionHandler<tSignalType >     tSuperType;

	tSuperType  mSuper;
};

}

#endif // Signalv1ImplSerious.hxx
