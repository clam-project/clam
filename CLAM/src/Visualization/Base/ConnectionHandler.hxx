#ifndef __CONNECTIONHANDLER__
#define __CONNECTIONHANDLER__

#include <list>
#include <utility>
#include "Connection.hxx"

namespace CLAMGUI
{

template < class SignalType >
class ConnectionHandler
{
public:
		typedef typename SignalType::tCallbackType                 tCallbackType;
		typedef typename SignalType::tConnectionId                 tConnectionId;


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
		
public:
		
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

private:
		
		tCallList       mCalls;
		tCallbackList   mCallbacks;
		
};

}

#endif
