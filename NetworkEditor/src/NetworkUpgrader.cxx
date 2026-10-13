#ifdef CLAM_NETWORKEDITOR_USE_PYTHON
#include "Python.h"
#endif

#include "NetworkUpgrader.hxx"

#ifdef CLAM_NETWORKEDITOR_USE_PYTHON

#include <CLAM/EmbeddedFile.hxx>
#include <CLAM/Assert.hxx>
#include <iostream>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
namespace py=boost::python;

// The build system gives the absolute paths of the scripts to embed; the
// defaults are the ones the SCons build relies on (its working directory
// and the installed CLAM).
#ifndef NETWORKEDITOR_MIGRATION_SCRIPT
#define NETWORKEDITOR_MIGRATION_SCRIPT "migrationScript"
#endif
#ifndef CLAM_REFACTOR_SCRIPT
#define CLAM_REFACTOR_SCRIPT CLAM_PREFIX "/share/clam/clamrefactor.py"
#endif
CLAM_EMBEDDED_FILE(migrationScript, NETWORKEDITOR_MIGRATION_SCRIPT)
CLAM_EMBEDDED_FILE(clamrefactor, CLAM_REFACTOR_SCRIPT)

class NetworkUpgrader::Impl
{
public:
	std::string _result;
	std::string _errorMessage;
	const char * run(const std::string & networkContents);
	const char * takePythonError(const std::string & context);
	const char * setError(const std::string & error)
	{
		_errorMessage = error;
		return 0;
	}
};


NetworkUpgrader::NetworkUpgrader()
	: _impl(new Impl)
{
}
NetworkUpgrader::~NetworkUpgrader()
{
	delete _impl;
}

const char * NetworkUpgrader::run(const std::string & filename)
{
	return _impl->run(filename);
}

const char * NetworkUpgrader::errorMessage() const
{
	return _impl->_errorMessage.c_str();
}


const char * NetworkUpgrader::Impl::takePythonError(const std::string & context="")
{
	PyObject * pType=nullptr;
	PyObject * pValue=nullptr;
	PyObject * pTrace=nullptr;
	PyErr_Fetch(&pType, &pValue, &pTrace);
	PyErr_NormalizeException(&pType, &pValue, &pTrace);

	_errorMessage = context;
	try
	{
		// Any of the three may be null; a null handle is not a valid object
		py::object type = pType ? py::object(py::handle<>(pType)) : py::object();
		py::object value = pValue ? py::object(py::handle<>(pValue)) : py::object();
		py::object trace = pTrace ? py::object(py::handle<>(pTrace)) : py::object();
		py::object traceback = py::import("traceback");
		py::str text = py::str("").join(
			traceback.attr("format_exception")(type, value, trace));
		_errorMessage += py::extract<std::string>(text);
	}
	catch (py::error_already_set &)
	{
		PyErr_Clear();
		_errorMessage += "Python error (the traceback could not be formatted)";
	}
	std::cerr << _errorMessage << std::endl;
	return 0;
}

const char * NetworkUpgrader::Impl::run(const std::string & filename)
{
	try
	{
		if (not Py_IsInitialized()) Py_Initialize();
		py::object _main = py::import("__main__");
		py::object _main_ns = _main.attr("__dict__");
		// Simulate that we have a working command line
		py::exec("import sys" , _main_ns);
		py::exec("sys.argv=['clamrefactor.py']\n", _main_ns);
		// Not __main__ so it is run as it was a module, not a script
		_main.attr("__name__") = "NetworkUpgrader";
		// Run the embedded script
		_main.attr("__file__") = "(binary embeded) clamrefactor.py";
		py::exec(clamrefactor, _main_ns);

		py::object network = _main.attr("ClamNetwork")(filename);
		network.attr("runScript")(std::string(migrationScript));
		py::object stringio = py::import("io").attr("StringIO")();
		network.attr("dump")(stringio);
		_result = py::extract<std::string>(stringio.attr("getvalue")());
		return _result.c_str();
	}
	catch (py::error_already_set & e)
	{
		return takePythonError();
	}
}

#else // CLAM_NETWORKEDITOR_USE_PYTHON not defined

NetworkUpgrader::NetworkUpgrader()
	: _impl(0)
{
}
NetworkUpgrader::~NetworkUpgrader()
{
}

const char * NetworkUpgrader::run(const std::string & filename)
{
	return 0;
}

const char * NetworkUpgrader::errorMessage() const
{
	return "Python support is disabled. Python is required to upgrade networks on the fly.";
}

#endif

