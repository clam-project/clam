/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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
