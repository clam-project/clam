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

#include "ProcessingController.hxx"
//#include "InPortAdapter.hxx"
//#include "OutPortAdapter.hxx"
//#include "InControlAdapter.hxx"
//#include "OutControlAdapter.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

#include <iostream>
namespace CLAMVM
{

ProcessingController::ProcessingController()
	: mObserved(0),
	  mConfig(0)
{
	SlotConfigureProcessing.Wrap( this, &ProcessingController::ConfigureProcessing );
}

ProcessingController::~ProcessingController()
{
	if (mConfig)
		delete mConfig;

//	InPortAdapterIterator itPortIn;
//	for ( itPortIn=mInPortAdapters.begin(); itPortIn!=mInPortAdapters.end(); itPortIn++)
//		delete *itPortIn;
//	OutPortAdapterIterator itPortOut;
//	for ( itPortOut=mOutPortAdapters.begin(); itPortOut!=mOutPortAdapters.end(); itPortOut++)
//		delete *itPortOut;
//	InControlAdapterIterator itCtrlIn;
//	for ( itCtrlIn=mInControlAdapters.begin(); itCtrlIn!=mInControlAdapters.end(); itCtrlIn++)
///		delete *itCtrlIn;
//	OutControlAdapterIterator itCtrlOut;
//	for ( itCtrlOut=mOutControlAdapters.begin(); itCtrlOut!=mOutControlAdapters.end(); itCtrlOut++)
//		delete *itCtrlOut;

}

void ProcessingController::ConfigureProcessing( CLAM::ProcessingConfig * cfg) 
{
	mConfig = (CLAM::ProcessingConfig*)cfg->DeepCopy();

	if (mObserved->GetExecState() == CLAM::Processing::Running)
	{
		mObserved->Stop();
		Update();
		mObserved->Start();
	}
	else
		Update();
}
	

bool ProcessingController::Publish()
{
	if ( !mObserved )  // there is no object being observed
		return false;
	
	SignalAcquireClassName.Emit( mObserved->GetClassName() );
	if (mConfig)
		delete mConfig;

	const CLAM::ProcessingConfig & conf( mObserved->GetConfig() );
	mConfig = (CLAM::ProcessingConfig*)conf.DeepCopy();
	SignalAcquireConfig.Emit( mConfig );
//	CLAM::Processing* mObserved = (CLAM::Processing*) mObserved;	
//	CLAM::PublishedInPorts::ConstIterator itPortIn;
//	for (itPortIn = mObserved->GetInPorts().Begin(); 
//	     itPortIn != mObserved->GetInPorts().End(); 
//	     itPortIn++)
//	{

//		std::cout << "publishing" << std::endl;
//		mInPortNames.push_back((*itPortIn)->GetName());
		
//		InPortAdapter* adapter = new InPortAdapter;
//		CLAM::InPort* inport = *itPortIn;
//		adapter->BindTo(*inport);
//		mInPortAdapters.push_back(adapter);
//		SignalAcquireInPort.Emit(adapter);
//	}
	
//	CLAM::PublishedOutPorts::ConstIterator itPortOut;
//	for (itPortOut = mObserved->GetOutPorts().Begin(); 
//	     itPortOut != mObserved->GetOutPorts().End(); 
//	     itPortOut++)
//	{		
//		mOutPortNames.push_back((*itPortOut)->GetName());

//		OutPortAdapter* adapter = new OutPoirtAdapter;
//		CLAM::OutPort* outport = *itPortOut;
//		adapter->BindTo(*outport);
//		mOutPortAdapters.push_back(adapter);
//		SignalAcquireOutPort.Emit(adapter);
//	}


//	CLAM::PublishedInControls::ConstIterator itCtrlIn;
//	for (itCtrlIn = mObserved->GetInControls().Begin(); 
//	     itCtrlIn != mObserved->GetInControls().End(); 
//	     itCtrlIn++)
//	{
//		mInControlNames.push_back((*itCtrlIn)->GetName());

//		InControlAdapter* adapter = new InControlAdapter;
//		CLAM::InControl* incontrol = *itCtrlIn;
//		adapter->BindTo(*incontrol);
//		mInControlAdapters.push_back(adapter);
//		SignalAcquireInControl.Emit(adapter);
//	}
	
//	CLAM::PublishedOutControls::ConstIterator itCtrlOut;
//	for (itCtrlOut = mObserved->GetOutControls().Begin(); 
//	     itCtrlOut != mObserved->GetOutControls().End(); 
//	     itCtrlOut++)
//	{		
//		mOutControlNames.push_back((*itCtrlOut)->GetName());

//		OutControlAdapter* adapter = new OutControlAdapter;
//		CLAM::OutControl* outcontrol = *itCtrlOut;
//		adapter->BindTo(*outcontrol);
//		mOutControlAdapters.push_back(adapter);
//		SignalAcquireOutControl.Emit(adapter);
//	}

	return true;
}

bool ProcessingController::BindTo( CLAM::Processing& obj )
{
	mObserved = dynamic_cast< CLAM::Processing* > (&obj);
	
	if ( !mObserved )
	{ 
		return false;
	}

	CLAM::PublishedInPorts::ConstIterator itPortIn;
	for (itPortIn = mObserved->GetInPorts().Begin(); itPortIn != mObserved->GetInPorts().End(); itPortIn++)
		mInPortNames.push_back((*itPortIn)->GetName());
		
	CLAM::PublishedOutPorts::ConstIterator itPortOut;
	for (itPortOut = mObserved->GetOutPorts().Begin(); itPortOut != mObserved->GetOutPorts().End(); itPortOut++)
		mOutPortNames.push_back((*itPortOut)->GetName());


	CLAM::PublishedInControls::ConstIterator itCtrlIn;
	for (itCtrlIn = mObserved->GetInControls().Begin(); itCtrlIn != mObserved->GetInControls().End(); itCtrlIn++)
		mInControlNames.push_back((*itCtrlIn)->GetName());
	
	CLAM::PublishedOutControls::ConstIterator itCtrlOut;
	for (itCtrlOut = mObserved->GetOutControls().Begin(); itCtrlOut != mObserved->GetOutControls().End(); itCtrlOut++)
		mOutControlNames.push_back((*itCtrlOut)->GetName());

	return true;
}

bool ProcessingController::Update()
{
	mObserved->Configure( *mConfig );

	CheckIfLadspaLoader();
	return true;
}

void ProcessingController::CheckIfLadspaLoader()
{
	if(std::string(mObserved->GetClassName())=="LadspaLoader")
	{
		SignalRebuildProcessingStructure.Emit( mObserved );
		SignalRemoveProcessingController.Emit(this );
	}
	
}

} //namespace CLAMVM
