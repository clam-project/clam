
#include "MainWindow.hxx"
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qbutton.h>
#include <qlayout.h> 
#include <iostream>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>

#include <string>

namespace NetworkGUI
{

MainWindow::MainWindow()
	: mNetwork(this),
	  mFactory( this , "factory" ),
	  mNetworkActions( this ),
	  QMainWindow( 0, "", WGroupLeader )
{
	mNetwork.setFocus();	
	mFactory.move(640, 40);
	mNetworkActions.move(640, 300);

	setCentralWidget( &mNetwork );
	setCaption( "CLAM Network Editor" );
	resize( 800, 600 );
	
	NewMessageToStatus.Wrap( this, &MainWindow::OnNewMessageToStatus );
	mNetwork.SendNewMessageToStatus.Connect( NewMessageToStatus );
	statusBar()->message( "Ready to edit" );

	mFactory.AddNewProcessing.Connect( mNetwork.AddNewProcessing );
	mNetworkActions.ChangeNetworkState.Connect( mNetwork.ChangeState );


	QPopupMenu * file = new QPopupMenu( this );
        menuBar()->insertItem( "&File", file );


	setCentralWidget(&mNetwork);
 
	file->insertItem("New", this, SLOT(NewNetwork()));
	file->insertItem("Load", this, SLOT(LoadNetwork()));
	file->insertItem("Save", this, SLOT(SaveNetwork()));
	file->insertItem("Save As", this, SLOT(SaveAsNetwork()));

}

MainWindow::~MainWindow()
{
}

void MainWindow::OnNewMessageToStatus( const std::string & message)
{
	statusBar()->message( QString( message.c_str() ), 2000);
}

Qt_NetworkPresentation & MainWindow::GetNetworkPresentation()
{
	return mNetwork;
}

void MainWindow::Start()
{
	std::cout << "starting network" << std::endl;
}

void MainWindow::Stop()
{
	std::cout << "stopping network" << std::endl;
}

void MainWindow::NewNetwork()
{
	std::cout << "new network" << std::endl;
}

void MainWindow::LoadNetwork()
{
/*
	QFileDialog* fd = new QFileDialog( this, "file dialog", TRUE );
	fd->setMode( QFileDialog::AnyFile);
	fd->addFilter( "XML Files (*.xml)" );
	fd->show();
*/

	QString s = QFileDialog::getOpenFileName(
		"",
		"XML Files (*.xml)",
		this,
		"open file dialog"
		"Choose a file to load network" );
	if (s!=QString::null)
		mNetwork.LoadNetworkFrom.Emit(std::string(s.ascii()));
}


void MainWindow::SaveNetwork()
{
	QString s = QFileDialog::getSaveFileName(
		"",
		"XML Files (*.xml)",
		this,
		"save file dialog"
		"Choose a file to save network" );

	if (s!=QString::null)
		mNetwork.SaveNetworkTo.Emit(std::string(s.ascii()));
}

void MainWindow::SaveAsNetwork()
{
	std::cout << "saving network as" << std::endl;
}

} // namespace NetworkGUI
