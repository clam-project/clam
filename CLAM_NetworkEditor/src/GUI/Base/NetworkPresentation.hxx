
#ifndef __NETWORKPRESENTATION_HXX__
#define __NETWORKPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"
#include "Slotv3.hxx"
#include "Signalv2.hxx"

#include <string>
#include <list>


namespace CLAMVM
{
	class NetworkModel;
	class ProcessingAdapter;
	class ConnectionAdapter;
}

namespace NetworkGUI
{

class ProcessingPresentation;
class ConnectionPresentation;
class OutPortPresentation;
class InPortPresentation;

class NetworkPresentation: public CLAMVM::Presentation
{
protected:
	std::string mName;
	std::list<ProcessingPresentation*> mProcessingPresentations;
	typedef std::list<ProcessingPresentation*>::iterator ProcessingPresentationIterator;
	std::list<ConnectionPresentation*> mConnectionPresentations;
	typedef std::list<ConnectionPresentation*>::iterator ConnectionPresentationIterator;

public:
	NetworkPresentation();
        virtual ~NetworkPresentation();
	virtual void AttachTo(CLAMVM::NetworkModel &);
	virtual void Show() = 0;
	virtual void Hide() = 0;

protected:
	virtual void OnNewName(const std::string& name) = 0; 
	virtual void OnNewProcessing(CLAMVM::ProcessingAdapter*, const std::string & ) = 0;
	virtual void OnNewConnection(CLAMVM::ConnectionAdapter* ) = 0;
	virtual void OnRemoveConnection(const std::string &, const std::string &,ConnectionPresentation*);

	// methods related to locate processing
	OutPortPresentation & GetOutPortPresentationByCompleteName(const std::string &);
	InPortPresentation & GetInPortPresentationByCompleteName(const std::string &);
	ProcessingPresentation& GetProcessingPresentation( const std::string & );
	std::string GetProcessingIdentifier( const std::string& );
	static char NamesIdentifiersSeparator();
	static std::size_t PositionOfLastIdentifier( const std::string & );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );

public: //slots
	SigSlot::Slotv1<const std::string& > SetName;
	SigSlot::Slotv2< CLAMVM::ProcessingAdapter*, const std::string & > SetProcessing;
	SigSlot::Slotv1< CLAMVM::ConnectionAdapter* > SetConnection;
	SigSlot::Slotv3< const std::string &, const std::string &, ConnectionPresentation* > SetRemoveConnection;
	
	//signals
	SigSlot::Signalv2< const std::string &, const std::string & > CreateNewConnectionFromGUI;
	SigSlot::Signalv2< const std::string &, const std::string & > RemoveConnectionFromGUI;

};

} // namespace NetworkGUI

#endif // __NETWORKPRESENTATION_HXX__
