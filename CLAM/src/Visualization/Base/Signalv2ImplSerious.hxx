#ifndef __SIGNALV2IMPLSERIOUS__
#define __SIGNALV2IMPLSERIOUS__

#ifndef __SIGNALV2__
#error "This is an internal implementation header. You are not allowed to include it directly!"
#endif

#include "ConnectionHandler.hxx"

namespace CLAMGUI
{

template < typename ParmType1, template ParmType2 >
	class Signalv2 : public Signal
{
public:
	typedef typename CBL::Functor1<ParmType1,ParmType2>              tCallbackType;

public:
	
	template < class RefType, typename PtrMember >
		void Connect( RefType thisRef, PtrMember pMember, Slot& slot )
	{
		Connection c( AssignConnection(), this );

		mSuper.AddCallback( c.GetID(), CBL::makeFunctor( (CBL::Functor2<ParmType1,ParmType2>*)0, *thisRef, pMember ) );

		slot.Bind(c);
	}

	template < typename PtrFunction >
		void Connect( PtrFunction pMember, Slot& slot )
	{
		Connection c( AssignConnection(), this );

		mSuper.AddCallback( s.GetID(), CBL::makeFunctor( (CBL::Functor2<ParmType2>*)0, pMember ) );

		slot.Bind(c);
	}
	
	void Emit( ParmType1 parm1, ParmType2 parm2 )
	{
		if ( mSuper.HasNoCallbacks() )
			return;
		
		tSuperType::tCallList calls = mSuper.GetCalls();
		tSuperType::tCallIterator i = calls.begin();
		tSuperType::tCallIterator end = calls.end();

		while ( i != end )
			{
				(*(*i))( parm1, parm2 );
				i++;
			}
		
	}

	void FreeConnection( Connection* pConnection )
	{
		mSuper.RemoveCall( pConnection->GetID() );
		FreeConnectionId( pConnection->GetID() );
	}
private:
	typedef Signalv1<ParmType1,ParmType2>                tSignalType;
	typedef ConnectionHandler<tSignalType >     tSuperType;

	tSuperType  mSuper;
};

}


#endif // Signalv2ImplSerious.hxx
