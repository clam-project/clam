#ifndef __SIGNALV0IMPLSERIOUS__
#define __SIGNALV0IMPLSERIOUS__

#ifndef __SIGNALV0__
#error "This is an internal implementation header. You are not allowed to include it directly!"
#endif


#include "ConnectionHandler.hxx"

namespace CLAMGUI
{

		class Signalv0 
				: public Signal
		{
		public:
				typedef typename CBL::Functor0     tCallbackType;
		public:
				virtual ~Signalv0()
				{
						mSuper.DestroyConnections();
				}
				
				template < class RefType, typename PtrMember >
				void Connect( RefType thisRef, PtrMember pMember, Slot& slot )
				{
						Connection c ( AssignConnection(), this );

						mSuper.AddCallback( c.GetID(), &slot, CBL::makeFunctor( (CBL::Functor0*)0, *thisRef, pMember ) );
						
						slot.Bind(c);
				}

				template < class PtrFunction >
				void Connect( PtrFunction pMember, Slot& slot )
				{
						Connection c ( AssignConnection(), this );

						mSuper.AddCallback( c.GetID(), &slot, CBL::makeFunctor( (CBL::Functor0*)0, pMember ) ); 

						slot.Bind(c);
				}

				void Emit()
				{
						if ( HasNoCallbacks() )
								return;

						tSuperType::tCallbackList calls = GetCalls();
						tSuperType::tCallIterator i = calls.begin();
						tSuperType::tCallIterator end = calls.end();

						while( i != end )
						{
								(*(*i))();
								i++;
						}
				}

				void FreeConnection( Connection* pConnection )
				{
						mSuper.RemoveCall( pConnection->GetID() );
						FreeConnectionId( pConnection->GetID() );
				}

		private:
				typedef Signalv0                            tSignalType;
				typedef ConnectionHandler<tSignalType >     tSuperType;
				
				tSuperType  mSuper;
		};

}

#endif // Signalv0.hxx
