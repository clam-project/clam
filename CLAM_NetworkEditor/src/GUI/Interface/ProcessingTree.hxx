
#ifndef __PROCESSINGTREE_HXX__
#define __PROCESSINGTREE_HXX__

#include <qlistview.h>
#include "NetworkPresentation.hxx"

namespace NetworkGUI
{

class ProcessingTree : public QListView
{
public:
	ProcessingTree( NetworkPresentation & network, QWidget * parent = 0, const char * name = 0 );
	virtual ~ProcessingTree();

};

} // namespace NetworkGUI

#endif //  __PROCESSINGTREE_HXX__
