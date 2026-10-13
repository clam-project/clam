#include <QApplication>
#include <Oscilloscope.hxx>
#include <iostream>

// Builds a monitor widget from the installed clam_qtmonitors library.
int main(int argc, char ** argv)
{
	QApplication app(argc, argv);
	Oscilloscope scope;
	std::cout << scope.metaObject()->className() << std::endl;
	return 0;
}
