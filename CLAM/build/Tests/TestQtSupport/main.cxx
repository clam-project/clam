#include "MainWindow.hxx"
#include "ControlPanel.h"
#include "Assert.hxx"

int main( int argc, char** argv )
{
	NetworkGUI::MainWindow* widget = new NetworkGUI::MainWindow( );

	CLAM_ASSERT( widget != NULL, "Could not create widget!" );

	delete widget;

	return 0;
}
