#ifndef __SIGNALV1IMPLVC6__
#define __SIGNALV1IMPLVC6__

#ifndef __SIGNALV1__
#error "This is an implementation header. You are not allowed to include it directly!"
#endif

#include <list>
#include <utility>

namespace CLAMGUI
{

template < typename ParmType1 >
	class Signalv1 : public Signal
{
public:
		typedef typename CBL::Functor1<ParmType1>                  tCallbackType;
		typedef std::pair<tConnectionId, tCallbackType>                  tCallback;
		typedef tCallbackType*                                     tCallbackPtr;
		typedef std::list<tCallbackPtr>                            tCallList;
		typedef std::list<tCallbackPtr >::iterator                 tCallIterator;
		typedef std::list<tCallback>                               tCallbackList;
		typedef typename std::list<tCallback>::iterator            tCbListIterator;
		typedef typename std::list<tCallback>::const_iterator      const_tCbListIterator;

protected:		

		void AddCallback( tConnectionId pConnection, tCallbackType cb )
		{
				mCallbacks.push_back( tCallback( pConnection, cb ) );
		}
		
		bool HasNoCallbacks( ) const
		{
				return mCallbacks.empty();
		}
		
		tCallList& GetCalls( )
		{
				mCalls.clear();
				
				tCbListIterator i   = mCallbacks.begin();
				tCbListIterator end = mCallbacks.end();
				
				while ( i!=end)
				{
						mCalls.push_back( &(i->second) );
						i++;
				}
				
				return mCalls;
		}
		
		void RemoveCall(  tConnectionId id )
		{
				tCbListIterator i = mCallbacks.begin();
				tCbListIterator end = mCallbacks.end();

				while ( i!=end )
				{
						if ( i->first == id )
						{
								mCallbacks.erase( i );
								break;
						}
						i++;
				}
		}
		
		
public:
	
	template < class RefType, typename PtrMember >
		void Connect( RefType thisRef, PtrMember pMember, Slot& slot )
	{
		Connection c( AssignConnection(), this );

		AddCallback( c.GetID(), CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, *thisRef, pMember ) );

		slot.Bind(c);
	}

	template < typename PtrFunction >
		void Connect( PtrFunction pMember, Slot& slot )
	{
		Connection c( AssignConnection(), this );

		AddCallback( c.GetID(), CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, pMember ) );

		slot.Bind(c);
	}
	
	void Emit( ParmType1 parm )
	{
		if ( HasNoCallbacks() )
			return;
		
		tCallList calls = GetCalls();
		tCallIterator i = calls.begin();
		tCallIterator end = calls.end();

		while ( i != end )
			{
				(*(*i))( parm );
				i++;
			}
		
	}

	void FreeConnection( Connection* pConnection )
	{
		RemoveCall( pConnection->GetID() );
		FreeConnectionId( pConnection->GetID() );
	}

private:

		tCallList       mCalls;
		tCallbackList   mCallbacks;


};

}


#endif // Signalv1ImplVC6.hxx
