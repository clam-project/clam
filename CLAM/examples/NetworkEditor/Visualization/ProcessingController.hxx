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

#ifndef _PROCESSINGCONTROLLER_
#define _PROCESSINGCONTROLLER_

#include "ModelController.hxx"
//#include "ProcessingModel.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "Signalv0.hxx"
#include <string>

namespace CLAM
{
	class Processing;
	class ProcessingConfig;
}

namespace CLAMVM
{

//class InPortAdapter;
//class OutPortAdapter;
//class InControlAdapter;
//class OutControlAdapter;

class ProcessingController : public ModelController //, public ProcessingModel
{
public:
	typedef std::list<std::string> NamesList;
		
private:
	CLAM::Processing* mObserved;
	CLAM::ProcessingConfig* mConfig;

protected:
//	std::list<InPortAdapter*> mInPortAdapters;
//	std::list<OutPortAdapter*> mOutPortAdapters;
//	std::list<InControlAdapter*> mInControlAdapters;
//	std::list<OutControlAdapter*> mOutControlAdapters;
//	typedef std::list<OutPortAdapter*>::iterator OutPortAdapterIterator;
//	typedef std::list<InPortAdapter*>::iterator InPortAdapterIterator;
//	typedef std::list<OutControlAdapter*>::iterator OutControlAdapterIterator;
//	typedef std::list<InControlAdapter*>::iterator InControlAdapterIterator;
//	
	
	NamesList mInPortNames;
	NamesList mOutPortNames;
	NamesList mInControlNames;
	NamesList mOutControlNames;

	void ConfigureProcessing( CLAM::ProcessingConfig* );
	void CheckIfLadspaLoader();
public:
	ProcessingController();
	virtual ~ProcessingController();
	virtual const char* GetClassName() const
	{
		return "ProcessingController";
	}
	virtual bool Publish();
	virtual bool BindTo( CLAM::Processing& obj );
	virtual bool Update();

	NamesList::iterator BeginInPortNames()
	{
		return mInPortNames.begin();
	}
	
	NamesList::iterator EndInPortNames()
	{
		return mInPortNames.end();
	}
	
	NamesList::iterator BeginOutPortNames()
	{
		return mOutPortNames.begin();
	}
	
	NamesList::iterator EndOutPortNames()
	{
		return mOutPortNames.end();
	}

	NamesList::iterator BeginInControlNames()
	{
		return mInControlNames.begin();
	}
	
	NamesList::iterator EndInControlNames()
	{
		return mInControlNames.end();
	}
	
	NamesList::iterator BeginOutControlNames()
	{
		return mOutControlNames.begin();
	}
	
	NamesList::iterator EndOutControlNames()
	{
		return mOutControlNames.end();
	}
public:

	//signals & slots
	SigSlot::Signalv1< CLAM::ProcessingConfig *>  SignalAcquireConfig;
	SigSlot::Signalv1< const std::string &>          SignalAcquireClassName;
//	SigSlot::Signalv1< InPortAdapter* >   SignalAcquireInPort;
//	SigSlot::Signalv1< OutPortAdapter* > SignalAcquireOutPort;
//	SigSlot::Signalv1< InControlAdapter* >   SignalAcquireInControl;
//	SigSlot::Signalv1< OutControlAdapter* > SignalAcquireOutControl;
	SigSlot::Slotv1< CLAM::ProcessingConfig * > SlotConfigureProcessing;
	SigSlot::Signalv0 SignalUpdatePresentation;
	SigSlot::Signalv2< ProcessingController *, const std::string & > SignalCreateNewPresentation;
	SigSlot::Signalv1< CLAM::Processing * > SignalRebuildProcessingStructure;
	SigSlot::Signalv1< ProcessingController * > SignalRemoveProcessingController;

};

} // namespace CLAMVM

#endif // __PROCESSINGCONTROLLER__
