
#ifndef __PROCESSINGITEM_HXX__
#define __PROCESSINGITEM_HXX__

#include <qlistview.h> 
#include "Signalv2.hxx"
#include "NetworkPresentation.hxx"

namespace NetworkGUI
{

class ProcessingItem  : public QListViewItem
{
public:
	ProcessingItem( NetworkPresentation & network, QListView * parent = 0, const char* name = 0 );
	ProcessingItem( NetworkPresentation & network, QListViewItem * parent = 0, const char* name = 0 );
	virtual ~ProcessingItem();

protected:
	void activate();
	static int sNumProc;

public: //signals
	SigSlot::Signalv2 < const std::string &, CLAM::Processing * > AddNewProcessing;
};

} // namespace NetworkGUI

#endif // __PROCESSINGITEM_HXX__
