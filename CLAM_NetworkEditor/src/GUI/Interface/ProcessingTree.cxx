
#include "ProcessingTree.hxx"
#include "ProcessingItem.hxx"
#include "MainWindow.hxx"
#include "Factory.hxx"
#include <iostream>
#include <string>
#include <sstream>
#include <qheader.h>

namespace NetworkGUI
{

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
int ProcessingTree::sNumProc = 0;

ProcessingTree::ProcessingTree( NetworkPresentation & network, QWidget * parent, const char * name  )
	: QListView(parent, name)
{	
	addColumn( "Processing" );
	setRootIsDecorated( TRUE );
	QListViewItem * tmp1 = new QListViewItem( this,"Generators" );
	ProcessingItem * tmp2 = new ProcessingItem( tmp1, "Oscillator");
	QListViewItem * tmp3 = new QListViewItem( this,"Binary Operations" );
	ProcessingItem * tmp4 = new ProcessingItem( tmp3, "Multiplier" );
	ProcessingItem * tmp5 = new ProcessingItem( tmp3, "Mixer 2" );
	show();
	header()->hide();

	connect( this, SIGNAL( doubleClicked(QListViewItem *) ),
		 this, SLOT( CreateProcessing(QListViewItem *) ));

	AddNewProcessing.Connect( network.AddNewProcessing );	

}

ProcessingTree::~ProcessingTree()
{
}

void ProcessingTree::CreateProcessing(QListViewItem * item)
{
	if(dynamic_cast<ProcessingItem*>(item)==0)
		return;

	std::string className(item->text(0).ascii());

	std::stringstream name;
	name.str("");
	name << className;
	name << "_" << sNumProc;
	sNumProc++;
	ProcessingFactory & factory = ProcessingFactory::GetInstance();

	AddNewProcessing.Emit( name.str(), factory.Create(className) );
	
}


} // namespace NetworkGUI
