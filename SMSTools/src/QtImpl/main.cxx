#include <QApplication>
#include "QtSMSTools.hxx"

#ifdef WIN32
	#include <CLAM/InitAudioIO.hxx>
	#include <CLAM/InitProcessing.hxx>
#endif

int main( int argc, char ** argv )
{
#ifdef WIN32
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif

	QApplication app( argc, argv );

	QtSMS::QtSMSTools* w = new QtSMS::QtSMSTools;
	w->showMaximized();
	return app.exec();
}

// END

