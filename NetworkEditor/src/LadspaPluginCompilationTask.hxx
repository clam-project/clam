#ifndef LadspaPluginCompilationTask_hxx
#define LadspaPluginCompilationTask_hxx
#include "MainWindow.hxx"
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include "ui_LadspaMetadataEditor.h"


//////////////////////
// LADSPA Generator
// TODO: Use a TaskRunner
// TODO: Auto install the plugin into ~/.ladspa
// TODO: Windows?

static const char * ladspaCxxTemplate =
	"#include <CLAM/LadspaNetworkExporter.hxx>\n"
	"#include <CLAM/LadspaLibrary.hxx>\n"
	"#include <CLAM/EmbeddedFile.hxx>\n"
	"static CLAM::LadspaLibrary library;\n"
	"CLAM_EMBEDDED_FILE(embededNetwork,\"%1\")\n"
	"extern \"C\" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)\n"
	"{\n"
	"	static CLAM::LadspaNetworkExporter n2(library, embededNetwork, %2,\n"
	"			\"%3\", \"%4\",\n"
	"			\"%5\", \"%6\");\n"
	"	return library.pluginAt(index);\n"
	"}\n";

// The plugin is built with CMake against the CLAMConfig.cmake package
// installed with the CLAM libraries.
static const char * ladspaCMakeTemplate =
	"cmake_minimum_required(VERSION 3.22)\n"
	"project(ClamLadspaPlugin LANGUAGES CXX)\n"
	"set(CMAKE_CXX_STANDARD 17)\n"
	"set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
	"find_package(CLAM CONFIG REQUIRED)\n"
	"add_library(ladspa_plugin MODULE %1)\n"
	"set_target_properties(ladspa_plugin PROPERTIES\n"
	"	PREFIX \"\"\n"
	"	OUTPUT_NAME %2\n"
	"	LIBRARY_OUTPUT_DIRECTORY %3\n"
	")\n"
	"target_link_libraries(ladspa_plugin PRIVATE CLAM::core CLAM::processing CLAM::audioio)\n";

class LadspaPluginCompilationTask : public QObject
{
public:
	LadspaPluginCompilationTask(QWidget * parent, CLAM::Network & network)
		: QObject(parent)
		, _network(network)
	{
	}
	~LadspaPluginCompilationTask() {}
	void keepFiles()
	{
		_workDir.setAutoRemove(false);
	}
	/// The plugin file the last run() produced.
	QString pluginPath() const { return _pluginPath; }
	/// Returns the exit code of the failing cmake step (0 on success),
	/// -2 if cmake could not be started, or -3 if the intermediate files
	/// could not be written.
	int run(
		unsigned id, 
		const QString & label, 
		const QString & name, 
		const QString & maker, 
		const QString & copyright,
		const QString & libraryName,
		const QString & clamPrefix
		)
	{
		if (not _workDir.isValid()) return -3;
		std::cout << "LADSPA plugin sources in " << _workDir.path().toStdString() << std::endl;
		const QString networkFile = _workDir.filePath("network.clamnetwork");
		const QString cxxFile = _workDir.filePath("plugin.cxx");
		const QString buildDir = _workDir.filePath("build");

		std::ostringstream memoryBuffer;
		CLAM::XMLStorage::Dump(_network, "network", memoryBuffer);
		if (not writeFile(networkFile, QByteArray::fromStdString(memoryBuffer.str()))) return -3;

		QString pluginCode = QString(ladspaCxxTemplate)
			.arg(networkFile).arg(id)
			.arg(label).arg(name).arg(maker).arg(copyright);
		std::cout << pluginCode.toStdString() << std::endl;
		if (not writeFile(cxxFile, pluginCode.toUtf8())) return -3;

		// A relative library name is relative to the working directory,
		// as it was with the former SCons build.
		const QFileInfo library(libraryName);
		QString outputName = library.fileName();
		if (outputName.endsWith(".so")) outputName.chop(3);
		_pluginPath = library.absoluteDir().filePath(outputName + ".so");
		const QString cmakeCode = QString(ladspaCMakeTemplate)
			.arg(cmakeQuoted(cxxFile), cmakeQuoted(outputName), cmakeQuoted(library.absolutePath()));
		if (not writeFile(_workDir.filePath("CMakeLists.txt"), cmakeCode.toUtf8())) return -3;

		QStringList configure = QStringList()
			<< "-S" << _workDir.path()
			<< "-B" << buildDir
			<< "-DCMAKE_BUILD_TYPE=Release"
			;
		if (not clamPrefix.isEmpty())
			configure << "-DCMAKE_PREFIX_PATH=" + clamPrefix;
		std::cout << "cmake " << configure.join(" ").toStdString() << std::endl;
		const int configured = QProcess::execute("cmake", configure);
		if (configured != 0) return configured;
		return QProcess::execute("cmake", QStringList() << "--build" << buildDir);
	}	
private:
	static bool writeFile(const QString & path, const QByteArray & content)
	{
		QFile file(path);
		if (not file.open(QIODevice::WriteOnly)) return false;
		return file.write(content) == content.size();
	}
	static QString cmakeQuoted(QString value)
	{
		value.replace("\\", "\\\\").replace("\"", "\\\"").replace("$", "\\$");
		return "\"" + value + "\"";
	}
	CLAM::Network & _network;
	QTemporaryDir _workDir;
	QString _pluginPath;
};


#endif//LadspaPluginCompilationTask_hxx
