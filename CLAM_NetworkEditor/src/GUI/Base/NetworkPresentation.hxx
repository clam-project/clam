/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __NETWORKPRESENTATION_HXX__
#define __NETWORKPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv1.hxx"
#include "Slotv0.hxx"
#include "Slotv2.hxx"
#include "Signalv0.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"

#include <string>
#include <list>

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{
	class NetworkController;
	class ProcessingController;
	class ConnectionAdapter;
}

namespace NetworkGUI
{

class ProcessingPresentation;
class ConnectionPresentation;
class OutPortPresentation;
class InPortPresentation;
class OutControlPresentation;
class InControlPresentation;

class NetworkPresentation: public CLAMVM::Presentation
{
protected:
	std::string mName;
	bool mNetworkState;
	std::list<ProcessingPresentation*> mProcessingPresentations;
	typedef std::list<ProcessingPresentation*>::iterator ProcessingPresentationIterator;
	std::list<ConnectionPresentation*> mConnectionPresentations;
	typedef std::list<ConnectionPresentation*>::iterator ConnectionPresentationIterator;

public:
	NetworkPresentation();
        virtual ~NetworkPresentation();
	virtual void AttachTo(CLAMVM::NetworkController &);
	virtual void Show() = 0;
	virtual void Hide() = 0;

protected:
	virtual void SetName(const std::string& name) = 0; 
	virtual void ChangeState( bool );
	virtual void SetProcessing(CLAMVM::ProcessingController*, const std::string & ) = 0;
	virtual void SetPortConnection(CLAMVM::ConnectionAdapter* ) = 0;
	virtual void SetControlConnection(CLAMVM::ConnectionAdapter* ) = 0;
	virtual void SetRemovePortConnection( ConnectionPresentation* );
	virtual void SetRemoveControlConnection( ConnectionPresentation* );
	virtual void SetRemoveProcessing( ProcessingPresentation* );
	virtual void SetRemoveProcessingToGUI( const std::string & );
	virtual void AddNewProcessing( const std::string & , CLAM::Processing * );
	virtual void Clear( );


	// methods related to locate processing
	OutPortPresentation & GetOutPortPresentationByCompleteName(const std::string &);
	InPortPresentation & GetInPortPresentationByCompleteName(const std::string &);
	OutControlPresentation & GetOutControlPresentationByCompleteName(const std::string &);
	InControlPresentation & GetInControlPresentationByCompleteName(const std::string &);

	ProcessingPresentation& GetProcessingPresentation( const std::string & );
	std::string GetProcessingIdentifier( const std::string& );
	static char NamesIdentifiersSeparator();
	static std::size_t PositionOfLastIdentifier( const std::string & );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );

public: //slots
	SigSlot::Slotv1<const std::string& > SlotSetName;
	SigSlot::Slotv2< CLAMVM::ProcessingController*, const std::string & > SlotSetProcessing;
	SigSlot::Slotv1< CLAMVM::ConnectionAdapter* > SlotSetPortConnection;
	SigSlot::Slotv1< CLAMVM::ConnectionAdapter* > SlotSetControlConnection;
	SigSlot::Slotv1< bool > SlotChangeState;
	SigSlot::Slotv1< ConnectionPresentation* > SlotSetRemovePortConnection;
	SigSlot::Slotv1< ConnectionPresentation* > SlotSetRemoveControlConnection;
	SigSlot::Slotv1< ProcessingPresentation* > SlotSetRemoveProcessing;
	SigSlot::Slotv0 SlotClear;
	
	//signals
	SigSlot::Signalv2< const std::string &, const std::string & > SignalCreateNewPortConnectionFromGUI;
	SigSlot::Signalv2< const std::string &, const std::string & > SignalCreateNewControlConnectionFromGUI;

	SigSlot::Signalv1< bool > SignalChangeState;
	SigSlot::Signalv2< const std::string &, const std::string & > SignalRemovePortConnectionFromGUI;
	SigSlot::Signalv2< const std::string &, const std::string & > SignalRemoveControlConnectionFromGUI;
	SigSlot::Signalv1< const std::string & > SignalRemoveProcessingFromGUI;
	SigSlot::Slotv1< const std::string & > SlotRemoveProcessingToGUI;
	SigSlot::Signalv1< const std::string & > SignalLoadNetworkFrom;
	SigSlot::Signalv1< const std::string & > SignalSaveNetworkTo;
	SigSlot::Signalv0 SignalClearSignal;

	SigSlot::Slotv2< const std::string &, CLAM::Processing *  > SlotAddNewProcessing;
	SigSlot::Signalv2 < const std::string &, CLAM::Processing * > SignalAddProcessing;
};

} // namespace NetworkGUI

#endif // __NETWORKPRESENTATION_HXX__
