
#ifndef __PROCESSINGTREE_HXX__
#define __PROCESSINGTREE_HXX__

#include <qlistview.h>
#include <string>
#include "Signalv2.hxx"
#include "NetworkPresentation.hxx"

namespace CLAM
{
	class Processing;
}

namespace NetworkGUI
{

class ProcessingTree : public QListView
{
Q_OBJECT
public:
	ProcessingTree( NetworkPresentation & network, QWidget * parent = 0, const char * name = 0 );
	virtual ~ProcessingTree();

protected:
	static int sNumProc;

public slots:
void CreateProcessing(QListViewItem *);

public: //signals
	SigSlot::Signalv2 < const std::string &, CLAM::Processing * > AddNewProcessing;      

};

} // namespace NetworkGUI

#endif //  __PROCESSINGTREE_HXX__
