//#include <CLAM/Network.hxx>
//#include <CLAM/CLAMVersion.hxx>
//#include <CLAM/JACKNetworkPlayer.hxx>
#include <boost/python.hpp>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>


bool hasattr(boost::python::object obj, std::string const &attrName)
{
	return PyObject_HasAttrString(obj.ptr(), attrName.c_str());
}

namespace py = boost::python;

class IpyclamShell
{
	py::object _main;
	py::object _main_ns;
public:
	IpyclamShell()
		: _main(py::import("__main__"))
		, _main_ns(_main.attr("__dict__"))
	{
		exec("import sys; sys.path.append('.')" , _main_ns, _main_ns);
	}
	void setupClamEngine()
	{
		py::object proxy = py::import("ipyclam.clam").attr("Clam_Engine")();
		setupNetwork(proxy);
	}
/*
	void setupClamEngine(CLAM::Network & network)
	{
		py::object clamProxyModule = py::import("ipyclam");
		py::object proxy = py::object(py::ptr(&network));
		setupNetwork(proxy);
	}
*/
	void setupJackEngine()
	{
		py::object proxy = py::import("ipyclam.jack").attr("Jack_Engine")();
		setupNetwork(proxy);
	}
	void setupDummyEngine()
	{
		py::object proxy = py::import("ipyclam.dummy").attr("Dummy_Engine")();
		setupNetwork(proxy);
	}
	void setupEngine(const std::string & engine)
	{
		static const std::map<std::string, std::string> engineModules = {
			{"Clam_Engine", "ipyclam.clam"},
			{"Jack_Engine", "ipyclam.jack"},
			{"Dummy_Engine", "ipyclam.dummy"},
		};
		auto it = engineModules.find(engine);
		if (it == engineModules.end())
			throw std::runtime_error("Unknown engine: " + engine);
		std::cout << "Engine: " << engine << std::endl;
		py::object proxy = py::import(it->second.c_str()).attr(engine.c_str())();
		setupNetwork(proxy);
	}
private:
	void setupNetwork(py::object & proxy)
	{
		py::object networkClass = py::import("ipyclam").attr("Network");
		_main_ns["net"] = networkClass(proxy);
	}
public:
	void run()
	{
		std::string shellBanner =
			"Interactive Python console for CLAM ";// + std::string(CLAM::GetFullVersion()) + "\n";
		std::string shellHeader = "Start by typing 'net.' and pressing the tab key";

		// This needs to be defined for ipython
		py::exec("sys.argv=['ipyclam']\n", _main_ns, _main_ns);

		_main_ns["__ipyclam_banner"] = shellBanner;
		_main_ns["__ipyclam_header"] = shellHeader;

		py::exec(R"(
from traitlets.config.loader import Config
from IPython.terminal.embed import embed
config = Config()
config.TerminalInteractiveShell.prompt_in_template = 'ipyclam[\\#] > '
embed(config=config, header=__ipyclam_header, user_ns=dict(globals()))
)", _main_ns, _main_ns);
	}
};


int main( int argc, char ** argv )
{
	try
	{
		Py_Initialize();

		IpyclamShell shell;
		if (argc>1)
		{
			std::string option = argv[1];
			if (option=="--dummy")
				shell.setupEngine("Dummy_Engine");
			else if (option=="--jack")
				shell.setupEngine("Jack_Engine");
		}
		else 
			shell.setupEngine("Clam_Engine");

		shell.run();
	}
	catch (py::error_already_set & e)
	{
		std::cerr << "Run time Python error!" << std::endl;
		PyErr_Print();
	}
}



