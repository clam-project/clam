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

#ifndef _NETWORKMODEL_HXX_
#define _NETWORKMODEL_HXX_

#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "Slotv0.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"
#include <string>

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{
	class ProcessingController;
	class ProcessingModel;
	class ConnectionAdapter;

	class NetworkModel
	{
	public:
		NetworkModel(){}
		virtual ~NetworkModel(){}

		//signals
		SigSlot::Signalv1< const std::string& >  SignalAcquireName;
		SigSlot::Signalv2< CLAMVM::ProcessingController* , const std::string& > SignalAcquireProcessing;
		SigSlot::Slotv2< CLAMVM::ProcessingModel *, const std::string & > SlotCreateNewPresentation;
		SigSlot::Signalv1< CLAMVM::ConnectionAdapter* > SignalAcquirePortConnection;
		SigSlot::Signalv1< CLAMVM::ConnectionAdapter* > SignalAcquireControlConnection;
		SigSlot::Signalv1< const std::string & > SignalRemoveProcessingToGUI;
		//slots
		SigSlot::Slotv1< bool > SlotChangeState;
		SigSlot::Slotv1< const std::string & > SlotRemoveProcessing;
		SigSlot::Slotv2< const std::string &, const std::string & > SlotRemovePortConnection;
		SigSlot::Slotv2< const std::string &, const std::string & > SlotRemoveControlConnection;
		SigSlot::Slotv2< const std::string &, CLAM::Processing *  > SlotAddNewProcessing;
		SigSlot::Slotv1< CLAM::Processing * > SlotRebuildProcessingStructure;
		SigSlot::Slotv2< const std::string &, const std::string& > SlotCreateNewPortConnection;
		SigSlot::Slotv2< const std::string &, const std::string& > SlotCreateNewControlConnection;
		SigSlot::Slotv1< const std::string & > SlotLoadNetwork;
		SigSlot::Slotv1< const std::string & > SlotSaveNetwork;
		SigSlot::Slotv1< CLAMVM::ProcessingModel * > SlotRemoveProcessingModel;
		SigSlot::Slotv0 SlotClear;
		
	};	
}

#endif // _NETWORKMODEL_HXX_
