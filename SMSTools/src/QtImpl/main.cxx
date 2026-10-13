#include <QApplication>
#include "QtSMSTools.hxx"

int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	QtSMS::QtSMSTools* w = new QtSMS::QtSMSTools;
	w->showMaximized();
	return app.exec();
}

// END

