#include "BackendConfigDialog.hxx"
#include <QApplication>
#include <QFile>
#include <QTemporaryDir>
#include <iostream>
#include <stdexcept>

static void check(bool condition, const char * message)
{
	if (!condition) throw std::runtime_error(message);
}

static QComboBox * combo(BackendConfigDialog & dialog, const char * name)
{
	auto result = dialog.findChild<QComboBox *>(QString::fromLatin1(name));
	check(result, "Missing selector");
	return result;
}

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);
	try
	{
		// Exercise discovery without a JACK server, including opposite directions,
		// MIDI ports, spaces, and characters which have meaning in regex patterns.
		QTemporaryDir commands;
		check(commands.isValid(), "Could not create command directory");
		QFile jack(commands.filePath("jack_lsp"));
		check(jack.open(QIODevice::WriteOnly), "Could not write jack_lsp fixture");
		jack.write(
			"#!/bin/sh\ncat <<'PORTS'\n"
			"Interface [USB]:capture_1\n   properties: output,physical,terminal,\n   32 bit float mono audio\n"
			"Interface [USB]:capture_2\n   properties: output,physical,terminal,\n   32 bit float mono audio\n"
			"Speakers:playback_1\n   properties: input,physical,terminal,\n   32 bit float mono audio\n"
			"Headphones:playback_2\n   properties: input,physical,terminal,\n   32 bit float mono audio\n"
			"Keyboard:midi\n   properties: output,physical,terminal,\n   8 bit raw midi\nPORTS\n");
		jack.close();
		check(jack.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner), "Could not make fixture executable");
		QFile pw(commands.filePath("pw-cli"));
		check(pw.open(QIODevice::WriteOnly), "Could not write pw-cli fixture");
		pw.write("#!/bin/sh\nexit 0\n");
		pw.close();
		pw.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
		qputenv("PATH", commands.path().toLocal8Bit() + ":" + qgetenv("PATH"));

		const QStringList sources = {"Capture_1", "Capture_2"};
		const QStringList sinks = {"Playback_1", "Playback_2"};
		BackendConfigDialog dialog("JACK", "", "", nullptr, sources, sinks);
		dialog.show();
		app.processEvents();
		check(dialog.usesJackPortConnections(), "New JACK settings should use per-channel selection");
		auto capture1 = combo(dialog, "capture1");
		auto playback1 = combo(dialog, "playback1");
		check(capture1->isVisible() && !combo(dialog, "inputPattern")->isVisible(), "Per-channel mode must display channel selectors");
		check(capture1->count() == 3 && playback1->count() == 3, "Port discovery must filter audio and direction");
		check(capture1->findData("Speakers:playback_1") < 0, "Capture must only offer JACK output ports");
		check(playback1->findData("Interface [USB]:capture_1") < 0, "Playback must only offer JACK input ports");
		check(dialog.sourceConnections().value("Capture_1").isEmpty(), "Blank channels must remain manual");
		capture1->setCurrentIndex(capture1->findData("Interface [USB]:capture_2"));
		combo(dialog, "capture2")->setCurrentIndex(1);
		playback1->setCurrentIndex(playback1->findData("Headphones:playback_2"));
		combo(dialog, "playback2")->setCurrentIndex(1);
		check(dialog.sourceConnections().value("Capture_1") == "Interface [USB]:capture_2", "Capture channel mapping was lost");
		check(dialog.sinkConnections().value("Playback_1") == "Headphones:playback_2", "Playback channel mapping was lost");

		// Typing over a selected item must use the new text instead of stale data.
		playback1->setEditText("Offline [device]:port.2");
		check(dialog.sinkConnections().value("Playback_1") == "Offline [device]:port.2", "Edited port name must override selected item data");
		dialog.findChild<QPushButton *>("refreshConnections")->click();
		check(dialog.sinkConnections().value("Playback_1") == "Offline [device]:port.2", "Refresh must retain unavailable targets");

		const auto savedSources = dialog.sourceConnections();
		const auto savedSinks = dialog.sinkConnections();
		BackendConfigDialog reopened("JACK", "", "", nullptr,
			{"Capture_2", "Capture_1"}, {"Playback_2", "Playback_1"}, savedSources, savedSinks);
		check(reopened.sourceConnections() == savedSources && reopened.sinkConnections() == savedSinks,
			"Reopening after channel reorder must preserve mappings by port name");
		combo(dialog, "backend")->setCurrentIndex(combo(dialog, "backend")->findData("PipeWire"));
		check(!dialog.usesJackPortConnections(), "Per-channel selection must be JACK-specific");
		check(!capture1->isVisible() && combo(dialog, "inputPattern")->isVisible(), "PipeWire must display node selectors");
		combo(dialog, "backend")->setCurrentIndex(combo(dialog, "backend")->findData("JACK"));
		check(dialog.sourceConnections() == savedSources && dialog.sinkConnections() == savedSinks,
			"Backend switching must preserve per-channel selections");

		BackendConfigDialog patterns("JACK", "^capture:.*", "^playback:.*", nullptr, sources, sinks);
		check(!patterns.usesJackPortConnections(), "Existing patterns must retain pattern mode");
		check(patterns.inputConnection() == "^capture:.*" && patterns.outputConnection() == "^playback:.*",
			"Existing pattern settings must survive discovery");
		auto outputPattern = combo(patterns, "outputPattern");
		outputPattern->setCurrentIndex(1);
		outputPattern->setEditText("^another:.*");
		check(patterns.outputConnection() == "^another:.*", "Edited patterns must override selected item data");
		combo(dialog, "jackRouting")->setCurrentIndex(1);
		check(!capture1->isVisible() && combo(dialog, "inputPattern")->isVisible(), "Pattern mode must display pattern fields");
		combo(dialog, "jackRouting")->setCurrentIndex(0);
		check(capture1->isVisible(), "Switching back must display channel selectors");

		check(jack.open(QIODevice::WriteOnly | QIODevice::Truncate), "Could not replace discovery fixture");
		jack.write("#!/bin/sh\nexit 1\n");
		jack.close();
		dialog.findChild<QPushButton *>("refreshConnections")->click();
		check(dialog.sourceConnections() == savedSources && dialog.sinkConnections() == savedSinks,
			"Discovery failure must preserve configured mappings");

		BackendConfigDialog empty("JACK", "", "");
		check(empty.sourceConnections().isEmpty() && empty.sinkConnections().isEmpty(), "Empty networks must be supported");
		std::cout << "Backend dialog routing checks passed\n";
	}
	catch (const std::exception & error)
	{
		std::cerr << error.what() << '\n';
		return 1;
	}
	return 0;
}
