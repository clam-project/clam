
#include "ProcessingTree.hxx"
#include "ProcessingItem.hxx"
#include "MainWindow.hxx"
#include <qheader.h>

#include <iostream>

namespace NetworkGUI
{

ProcessingTree::ProcessingTree( NetworkPresentation & network, QWidget * parent, const char * name  )
	: QListView(parent, name)
{	
	addColumn( "Processing" );
	setRootIsDecorated( TRUE );
	QListViewItem * tmp1 = new QListViewItem( this,"Generators" );
	ProcessingItem * tmp2 = new ProcessingItem( network, tmp1, "Oscillator");
	QListViewItem * tmp3 = new QListViewItem( this,"Binary Operations" );
	ProcessingItem * tmp4 = new ProcessingItem( network, tmp3, "Multiplier" );
	ProcessingItem * tmp5 = new ProcessingItem( network, tmp3, "Mixer 2" );
	show();
	header()->hide();

}

ProcessingTree::~ProcessingTree()
{
}

} // namespace NetworkGUI
