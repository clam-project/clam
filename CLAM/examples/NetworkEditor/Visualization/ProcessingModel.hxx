

#ifndef _PROCESSINGMODEL_
#define _PROCESSINGMODEL_

#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "Signalv0.hxx"
#include <string>

namespace CLAM
{
	class ProcessingConfig;
	class Processing;
}

namespace CLAMVM
{
	class InPortAdapter;
	class OutPortAdapter;
	class InControlAdapter;
	class OutControlAdapter;

	class ProcessingModel
	{
	public:		
		ProcessingModel(){}
		
		virtual ~ProcessingModel(){}
		//signals
		SigSlot::Signalv1< CLAM::ProcessingConfig *>  SignalAcquireConfig;
		SigSlot::Signalv1< const std::string &>          SignalAcquireClassName;
		SigSlot::Signalv1< InPortAdapter* >   SignalAcquireInPort;
		SigSlot::Signalv1< OutPortAdapter* > SignalAcquireOutPort;
		SigSlot::Signalv1< InControlAdapter* >   SignalAcquireInControl;
		SigSlot::Signalv1< OutControlAdapter* > SignalAcquireOutControl;
		SigSlot::Slotv1< CLAM::ProcessingConfig * > SlotConfigureProcessing;
		SigSlot::Signalv0 SignalUpdatePresentation;
		SigSlot::Signalv2< ProcessingModel *, const std::string & > SignalCreateNewPresentation;
		SigSlot::Signalv1< CLAM::Processing * > SignalRebuildProcessingStructure;
		SigSlot::Signalv1< ProcessingModel * > SignalRemoveProcessingModel;
	};
}

#endif // _PROCESSINGMODEL_
