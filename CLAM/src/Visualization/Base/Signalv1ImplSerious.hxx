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
	virtual ~Signalv1() {}

	template < class RefType, typename PtrMember >
		void Connect( RefType thisRef, PtrMember pMember, Slot& slot )
	{
		Connection c( AssignConnection(), this );

		mSuper.AddCallback( c.GetID(), CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, *thisRef, pMember ) );

		slot.Bind(c);
	}

	template < typename PtrFunction >
		void Connect( PtrFunction pMember, Slot& slot )
	{
		Connection c( AssignConnection(), this );

		mSuper.AddCallback( c.GetID(), CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, pMember ) );

		slot.Bind(c);
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

	void FreeConnection( Connection* pConnection )
	{
		mSuper.RemoveCall( pConnection->GetID() );
		FreeConnectionId( pConnection->GetID() );
	}
private:
	typedef Signalv1<ParmType1>                 tSignalType;
	typedef ConnectionHandler<tSignalType >     tSuperType;

	tSuperType  mSuper;
};

}

#endif // Signalv1ImplSerious.hxx
