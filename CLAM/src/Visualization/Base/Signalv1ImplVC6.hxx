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
// Begin of ConnectionHandler
		struct tCallback
		{
			tConnectionId  mConnection;
			Slot*          mSlot;
			tCallbackType  mCallback;

			tCallback( tConnectionId id, Slot* slot, tCallbackType cb )
				: mConnection( id ), mSlot( slot ), mCallback( cb )
			{
			}
		};

		typedef tCallbackType*                                     tCallbackPtr;
		typedef std::list<tCallbackPtr>                            tCallList;
		typedef typename std::list<tCallbackPtr >::iterator        tCallIterator;
		typedef std::list<tCallback>                               tCallbackList;
		typedef typename std::list<tCallback>::iterator            tCbListIterator;
		typedef typename std::list<tCallback>::const_iterator      const_tCbListIterator;


protected:		

		void AddCallback( tConnectionId pConnection, Slot* slot, tCallbackType cb )
		{
				mCallbacks.push_back( tCallback( pConnection, slot, cb ) );
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
						mCalls.push_back( &(i->mCallback) );
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
						if ( i->mConnection == id )
						{
								mCallbacks.erase( i );
								break;
						}
						i++;
				}
		}

		void DestroyConnections()
		{
			tCbListIterator elem;

			while ( !mCallbacks.empty() )
			{
				elem = mCallbacks.begin();

				elem->mSlot->Unbind( elem->mConnection );
			}
		}
// End of "ConnectionHandler"
		
public:
	
	virtual ~Signalv1()
	{
		DestroyConnections();
	}

	template < class RefType, typename PtrMember >
		void Connect( RefType thisRef, PtrMember pMember, Slot& slot )
	{
		Connection c( AssignConnection(), this );

		AddCallback( c.GetID(), &slot, CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, *thisRef, pMember ) );

		slot.Bind(c);
	}

	template < typename PtrFunction >
		void Connect( PtrFunction pMember, Slot& slot )
	{
		Connection c( AssignConnection(), this );

		AddCallback( c.GetID(), &slot, CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, pMember ) );

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
