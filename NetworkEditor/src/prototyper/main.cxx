#include "PrototypeLoader.hxx"
#include <QApplication>
#include <iostream>
#include <memory>
#include <QFileInfo>

#ifdef USE_LADSPA
#	include <CLAM/RunTimeLadspaLibraryLoader.hxx>
#	include <CLAM/RunTimeFaustLibraryLoader.hxx> 
#endif

#ifdef WIN32
#define WINDOWS_MEAN_AND_LEAN
#include <windows.h>
#define sleep(n) Sleep(1000*(n))
#else
#include <unistd.h>
#endif

int usage(const std::string & program, int status = 1)
{
		std::cout << " Usage: " << program
			<< " [<networkfile> [<interface.qml|interface.ui>]] [-o] [-b <backend> [-b <backend> ...]]\n"
			<< "Options:\n"
			<< " -o             Non interactive mode\n"
			<< " -b <backend>   Try backend (pipewire, jack, portaudio).\n"
			<< " -h, --help     Show this help.\n"
			<< " --start-paused  Pause just at start.\n"
			<< std::endl;
		return status;
}

int main( int argc, char *argv[] )
{

#ifdef USE_LADSPA
	RunTimeLadspaLibraryLoader ladspaLoader;
	ladspaLoader.Load();
	RunTimeFaustLibraryLoader faustLoader;
	faustLoader.Load();
#endif

	std::string networkFile;
	std::string uiFile;
	std::list<std::string> backends;
	bool isInteractive = true;
	bool startPaused = false;

	enum { none, backend } optionArgument = none;
	int argument=0;
	for (int i=1; i<argc; i++)
	{
		std::string arg = argv[i];
		if (optionArgument!=none)
		{
			if (optionArgument==backend)
			   	backends.push_back(arg);
			optionArgument=none;
			continue;
		}
		if (!arg.empty() && arg[0]=='-')
		{
			if (arg=="--help" || arg=="-h") return usage(argv[0], 0);
			if (arg=="-b")
			{
				optionArgument=backend;
				continue;
			}
			if (arg=="-o")
			{
				isInteractive=false;
				continue;
			}
			if (arg=="--start-paused")
			{
				startPaused=true;
				continue;
			}
			std::cerr << "Invalid option '" << arg << "'." << std::endl;
			return usage(argv[0]);
		}
		switch (argument++)
		{
			case 0:
				networkFile = arg;
			break;
			case 1:
				uiFile = arg;
			break;
			default:
				std::cerr << "Too many arguments." << std::endl;
				return usage(argv[0]);
		}
	}
    if (optionArgument != none) {
        std::cerr << "Option -b requires a backend name.\n";
        return usage(argv[0]);
    }
    if (!isInteractive && networkFile.empty()) {
        std::cerr << "Noninteractive mode requires a network file.\n";
        return usage(argv[0]);
    }
    const QString undottedName = networkFile.empty() ? QString("Prototyper")
        : QFileInfo(QString::fromStdString(networkFile)).completeBaseName();
    // Application outlives the loader, its windows, timers and network.
    std::unique_ptr<QCoreApplication> app(isInteractive ?
        static_cast<QCoreApplication *>(new QApplication(argc, argv)) : new QCoreApplication(argc, argv));
	CLAM::PrototypeLoader prototype;
	prototype.UseGUI(isInteractive);
	if (! prototype.ChooseBackend( backends, undottedName.toLocal8Bit().constData() ) ) return -1;
	if (! prototype.LoadNetwork( networkFile ) ) return -1;
	if (isInteractive)
	{
		if (! prototype.LoadInterface( uiFile.c_str() ) ) return -1;
		if (!prototype.ConnectUiWithNetwork()) return 1;
		prototype.Show();
	}
	else
		std::cout << QObject::tr("Non interface mode set. Press Ctrl-C to end.").toLocal8Bit().constData() << std::endl;
	if (!startPaused) prototype.Start();
	int result = app->exec();
	prototype.Stop();
	return result;
}
