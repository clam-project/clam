#include "IPyClamConsole.hxx"

#ifndef CLAM_USE_PYTHON
QWidget * GetIPyClamConsole(CLAM::Network & network)
{
	return 0; // Python disabled, no console
}
#else

#include <QHBoxLayout>
#undef slots
#undef signals
#include <boost/python.hpp>
#include <shiboken6/basewrapper.h>

namespace py=boost::python;

static void * error(const std::string & msg)
{
	std::cerr << msg << std::endl;
	return 0;
}

void * shibokenUnwrap(PyObject * pyobject)
{
	if (not Shiboken::Object::checkType(pyobject))
		return error("Not a shiboken object");

	SbkObject * sbkobject = (SbkObject *) pyobject;

	PyTypeObject * type = Py_TYPE(pyobject);

	void * cppobject = Shiboken::Object::cppPointer(sbkobject, type);
	if (not cppobject)
		return error("Not a QObject");

	return cppobject;
}

PyObject * shibokenWrap(QObject * qobject)
{
	return Shiboken::Object::newObject(
		Shiboken::ObjectType::typeForTypeName("QWidget"), qobject, false, true);
}

QWidget * GetIPyClamConsole(CLAM::Network & network)
{
	try
	{
		if (not Py_IsInitialized()) Py_Initialize();
		// Dummy __main__ namespace, to run execs
		py::object _main = py::import("__main__");
		py::object _main_ns = _main.attr("__dict__");
		// Adding working dir to the Python search path
		py::exec("import sys; sys.path.append('.')" , _main_ns, _main_ns);
		// Simulate that we have a working command line (expected by IPython)
		py::exec("sys.argv=['ipyclam']\n", _main_ns, _main_ns);
		// Build an ipyclam network having the CLAM network as backend
		py::object ipyclamModule = py::import("ipyclam");
		py::object engine = py::object(py::ptr(&network)); // The engine
		py::object net = ipyclamModule.attr("Network")(engine); // The ipyclam network api
		// Creating the IPython based console widget

		py::object consoleModule = py::import("ipyclam.qtconsole");
		py::object console = consoleModule.attr("IPythonConsoleQtWidget")();
		py::object signal = console.attr("modelChanged").attr("emit");
		net.attr("__dict__")["_engine"].attr("setCallback")(signal);

		// Injecting the network into the namespace
		console.attr("namespace_inject")("net", net);

		// Unwrapping the PySide based qt console to use it as a abstract QWidget
		QWidget * consoleWidget = (QWidget*) shibokenUnwrap(console.ptr());
		return consoleWidget;
	}
	catch (py::error_already_set & e)
	{
		std::cerr << "Run time Python error!" << std::endl;
		PyErr_Print();
		return 0;
	}

}



#endif//CLAM_USE_PYTHON
