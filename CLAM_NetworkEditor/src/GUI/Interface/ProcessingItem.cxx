
#include "ProcessingItem.hxx"
#include "Factory.hxx"
#include <iostream>
#include <string>
#include <sstream>

namespace NetworkGUI
{

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
int ProcessingItem::sNumProc = 0;

ProcessingItem::ProcessingItem( NetworkPresentation & network, QListView * parent , const char* name )
	: QListViewItem( parent , 0 )
{
	setText( 0, name );
	AddNewProcessing.Connect( network.AddNewProcessing );
}

ProcessingItem::ProcessingItem( NetworkPresentation & network, QListViewItem * parent , const char* name )
	: QListViewItem( parent , 0 )
{
	setText( 0, name );
	AddNewProcessing.Connect( network.AddNewProcessing );	
}

ProcessingItem::~ProcessingItem()
{
}

void ProcessingItem::activate()
{
	std::string className(text(0).ascii());
	std::cout << "created a new " << className << std::endl;

	std::stringstream name;
	name.str("");
	name << className;
	name << "_" << sNumProc;
	sNumProc++;
	ProcessingFactory & factory = ProcessingFactory::GetInstance();
	AddNewProcessing.Emit( name.str(), factory.Create(className) );
}

} // namespace NetworkGUI
