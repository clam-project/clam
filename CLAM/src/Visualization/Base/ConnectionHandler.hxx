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
		typedef std::pair<tConnectionId, tCallbackType>            tCallback;
		typedef tCallbackType*                                     tCallbackPtr;
		typedef std::list<tCallbackPtr>                            tCallList;
		typedef typename std::list<tCallbackPtr >::iterator        tCallIterator;
		typedef std::list<tCallback>                               tCallbackList;
		typedef typename std::list<tCallback>::iterator            tCbListIterator;
		typedef typename std::list<tCallback>::const_iterator      const_tCbListIterator;
		
public:
		
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
		
private:
		
		tCallList       mCalls;
		tCallbackList   mCallbacks;
		
};

}

#endif
