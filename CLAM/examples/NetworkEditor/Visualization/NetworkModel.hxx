
#ifndef _NETWORKMODEL_HXX_
#define _NETWORKMODEL_HXX_

#include "Signalv1.hxx"
#include "Signalv2.hxx"
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
	class ConnectionAdapter;

	class NetworkModel
	{
	public:
		NetworkModel(){}
		virtual ~NetworkModel(){}

		//signals
		SigSlot::Signalv1< const std::string& >  AcquireName;
		SigSlot::Signalv2< CLAMVM::ProcessingController* , const std::string& > AcquireProcessing;
		SigSlot::Signalv1< CLAMVM::ConnectionAdapter* > AcquirePortConnection;
		SigSlot::Signalv1< CLAMVM::ConnectionAdapter* > AcquireControlConnection;

		//slots
		SigSlot::Slotv1< bool > ChangeState;
		SigSlot::Slotv1< const std::string & > RemoveProcessing;
		SigSlot::Slotv2< const std::string &, const std::string & > RemovePortConnection;
		SigSlot::Slotv2< const std::string &, CLAM::Processing *  > AddNewProcessing;
		SigSlot::Slotv2< const std::string &, const std::string& > CreateNewPortConnection;
		SigSlot::Slotv2< const std::string &, const std::string& > CreateNewControlConnection;
		
	};	
}

#endif // _NETWORKMODEL_HXX_
