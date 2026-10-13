// SPDX-License-Identifier: GPL-2.0-or-later
#include "ControlBinding.hxx"
#include "Monitor.hxx"
#include "PrototypeLoader.hxx"
#include "QmlBindings.hxx"
#include "QtBinder.hxx"
#include <CLAM/ControlSource.hxx>
#include <QSlider>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/DiscontinuousSegmentation.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/LPModel.hxx>
#include <CLAM/MelCepstrum.hxx>
#include <CLAM/MelSpectrum.hxx>
#include <CLAM/Network.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/SpectralPeakArray.hxx>
#include <CLAM/Spectrum.hxx>
#include <CLAM/XMLStorage.hxx>
#include <QApplication>
#include <QElapsedTimer>
#include <QPainter>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QTemporaryFile>
#include <QThread>
#include <iostream>
#include <memory>
#include <stdexcept>

static void check(bool condition, const char *message) {
    if (!condition)
        throw std::runtime_error(message);
}
static void events(QApplication &app, int milliseconds) {
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < milliseconds) {
        app.processEvents();
        QThread::msleep(1);
    }
}
static std::unique_ptr<QObject> create(QQmlEngine &engine, const QByteArray &code) {
    QQmlComponent component(&engine);
    component.setData("import QtQuick\nimport CLAM.Widgets 1.0\n" + code, QUrl());
    while (component.isLoading())
        QCoreApplication::processEvents();
    if (component.isError())
        throw std::runtime_error(component.errorString().toStdString());
    std::unique_ptr<QObject> object(component.create());
    check(bool(object), "QML component failed to instantiate");
    return object;
}

class TestSource : public CLAM::Processing {
  public:
    CLAM::AudioOutPort audio{"Audio", this};
    CLAM::OutPort<CLAM::Audio> buffer{"Buffer", this};
    CLAM::OutPort<std::vector<CLAM::TData>> vector{"Vector", this};
    CLAM::OutPort<CLAM::Spectrum> spectrum{"Spectrum", this};
    CLAM::OutPort<CLAM::SpectralPeakArray> peaks{"Peaks", this};
    CLAM::OutPort<CLAM::LPModel> model{"Model", this};
    CLAM::OutPort<CLAM::MelSpectrum> mel{"Mel", this};
    CLAM::OutPort<CLAM::MelCepstrum> cepstrum{"Cepstrum", this};
    CLAM::OutPort<std::vector<std::pair<CLAM::TData, CLAM::TData>>> chroma{"Chroma", this};
    CLAM::OutPort<std::pair<CLAM::TData, CLAM::TData>> tuning{"Tuning", this};
    CLAM::OutPort<CLAM::DiscontinuousSegmentation> segmentation{"Segments", this};
    CLAM::InControl<float> gain{"Gain with spaces", this};
    CLAM::InControl<bool> enabled{"Enabled", this};
    CLAM::InControl<int> integer{"Integer", this};
    CLAM::InControl<std::string> filename{"Filename", this};
    CLAM::OutControl<float> progress{"Progress", this};
    CLAM::OutControl<bool> active{"Active", this};
    TestSource() { Configure(Config()); }
    const char *GetClassName() const override { return "QmlTestSource"; }
    bool Do() override {
        auto &frame = audio.GetAudio();
        for (int i = 0; i < frame.GetSize(); ++i)
            frame.GetBuffer()[i] = i % 2 ? .5 : -.5;
        audio.Produce();
        buffer.GetData() = frame;
        buffer.Produce();
        vector.GetData() = {0, .25, .5, 1};
        vector.Produce();
        spectrum.Produce();
        peaks.Produce();
        model.Produce();
        mel.Produce();
        cepstrum.Produce();
        chroma.GetData() = {{0, .5}, {7, 1}};
        chroma.Produce();
        tuning.GetData() = {.1, .05};
        tuning.Produce();
        segmentation.Produce();
        return true;
    }
};

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    try {
        // Regress both construction and timer updates before an interface exists.
        {
            CLAM::PrototypeLoader loader;
            loader.UseGUI(false);
            events(app, 550);
            check(loader.IsStopped(), "An empty loader must be stopped");
            check(loader.LoadInterface(QString()), "The default QML playback interface must load");
            check(loader.LoadInterface(
                      QStringLiteral(CLAM_SOURCE_DIR "/NetworkEditor/example-data/tonalAnalysis-file.ui")),
                  "Legacy widgets must load without an installed Designer plugin");
            check(!loader.LoadInterface("/missing/interface.qml"), "Explicit missing QML must fail");
            QTemporaryFile item;
            check(item.open(), "Could not create Item-root fixture");
            item.write("import QtQuick\nimport CLAM.Widgets 1.0\nItem { width: 320; height: 200; "
                       "Oscilloscope {} }\n");
            item.flush();
            // LoadInterface selects by extension.
            const QString path = item.fileName() + ".qml";
            check(QFile::copy(item.fileName(), path), "Could not copy Item-root fixture");
            check(loader.LoadInterface(path), "Prototyper must support a QML Item root");
            loader.Show();
            events(app, 20);
            QFile::remove(path);
        } // Windows and bindings must be destroyed before QApplication.

#ifdef USE_PORTAUDIO
        // PortAudio opens its device only on Start; exercise actual loader
        // bindings and orderly shutdown without starting audio hardware.
        for (const QString & interface : {QString("FilePlayer.ui"), QString("FilePlayer.qml")}) {
            CLAM::PrototypeLoader loader;
            loader.UseGUI(false);
            check(loader.ChooseBackend({"portaudio"}, "QmlWidgetsTest"), "PortAudio selection failed");
            check(loader.LoadNetwork(CLAM_SOURCE_DIR "/NetworkEditor/example-data/FilePlayer.clamnetwork"), "Example network failed to load");
            check(loader.LoadInterface(QString(CLAM_SOURCE_DIR "/NetworkEditor/example-data/") + interface), "Example interface failed to load");
            check(loader.ConnectUiWithNetwork(), "Example loader binding failed");
            loader.Show();
            events(app, 20);
        }
#endif

        CLAM::Network network;
        auto *source = new TestSource;
        network.AddProcessing("Source", source);
        QQmlEngine engine;
        engine.addImportPath(QStringLiteral(CLAM_QML_BUILD_DIR));
        const std::pair<const char *, const char *> monitors[] = {{"Oscilloscope", "Audio"},
                                                                  {"BufferOscilloscope", "Buffer"},
                                                                  {"Vumeter", "Audio"},
                                                                  {"SpectrumView", "Spectrum"},
                                                                  {"PeakView", "Peaks"},
                                                                  {"VectorView", "Vector"},
                                                                  {"HistogramView", "Audio"},
                                                                  {"LPModelView", "Model"},
                                                                  {"MelSpectrumView", "Mel"},
                                                                  {"MelCepstrumView", "Cepstrum"},
                                                                  {"Spectrogram", "Vector"},
                                                                  {"Tonnetz", "Vector"},
                                                                  {"KeySpace", "Vector"},
                                                                  {"ChordRanking", "Vector"},
                                                                  {"PolarChromaPeaks", "Chroma"},
                                                                  {"Tunner", "Tuning"},
                                                                  {"SegmentationView", "Segments"}};
        std::vector<std::unique_ptr<QObject>> widgets;
        for (const auto &entry : monitors) {
            auto widget =
                create(engine, QByteArray(entry.first) + " { width: 260; height: 140; clamOutPort: 'Source." +
                                   entry.second + "' }");
            QStringList errors;
            check(bindQmlInterface(widget.get(), network, errors), qPrintable(errors.join("\n")));
            widgets.push_back(std::move(widget));
        }
        network.Start();
        // Initial empty data must never index an empty vector or leave it frozen.
        for (const auto &widget : widgets)
            qobject_cast<ClamQuickMonitor *>(widget.get())->refresh();
        source->Do();
        for (auto it = network.BeginProcessings(); it != network.EndProcessings(); ++it)
            if (it->second != source)
                it->second->Do();
        for (const auto &widget : widgets) {
            auto *monitor = qobject_cast<ClamQuickMonitor *>(widget.get());
            monitor->refresh();
            QImage image(260, 140, QImage::Format_ARGB32_Premultiplied);
            image.fill(Qt::transparent);
            QPainter painter(&image);
            monitor->paint(&painter);
        }
        check(qobject_cast<ClamQuickMonitor *>(widgets[0].get())->sampleCount() > 0,
              "Audio samples must reach QML");
        check(qobject_cast<ClamQuickMonitor *>(widgets[5].get())->sampleCount() == 4,
              "Vector samples must reach QML");
        network.Stop();

        for (const char *component : {"ControlSlider", "ControlDial", "ControlSpinBox", "BoolControlSender",
                                      "ControlPrinter", "BoolControlDisplay", "ControlSurface", "MIDIPiano",
                                      "ProgressControl", "QFileLineEdit", "PlaybackControls", "QFirstPerson"})
            widgets.push_back(create(engine, QByteArray(component) + " {}"));

        auto slider = create(
            engine, "ControlSlider { clamInControl: 'Source.Gain with spaces'; from: 0; to: 2; value: 0.5 }");
        QStringList errors;
        check(bindQmlInterface(slider.get(), network, errors), qPrintable(errors.join("\n")));
        check(source->gain.GetLastValue() == .5f, "Initial values must reach controls");
        slider->setProperty("value", 1.25);
        check(source->gain.GetLastValue() == 1.25f, "QML control values use the declared range directly");
        auto boolean = create(engine, "BoolControlSender { clamInControl: 'Source.Enabled'; checked: true }");
        check(bindQmlInterface(boolean.get(), network, errors), "Boolean binding failed");
        check(source->enabled.GetLastValue(), "Boolean controls must retain their polarity");
        auto text = create(
            engine, "ControlBinding { clamInControl: 'Source.Filename'; value: 'file with spaces.wav' }");
        check(bindQmlInterface(text.get(), network, errors), "String binding failed");
        check(source->filename.GetLastValue() == "file with spaces.wav", "String control value was lost");

        auto observer = create(engine, "ControlBinding { clamOutControl: 'Source.Progress' }");
        check(bindQmlInterface(observer.get(), network, errors), "Output control binding failed");
        source->progress.SendControl(.75);
        events(app, 75);
        check(observer->property("observedValue").toDouble() == .75,
              "Output controls must reach QML observers");
        observer.reset();
        check(!source->progress.IsConnected(), "Destroying a UI must remove its observer link");

        auto bad = create(engine, "SpectrumView { clamOutPort: 'Source.Vector' }");
        const auto count = std::distance(network.BeginProcessings(), network.EndProcessings());
        errors.clear();
        check(!bindQmlInterface(bad.get(), network, errors) && !errors.isEmpty(),
              "Incompatible types must report a binding error");
        check(std::distance(network.BeginProcessings(), network.EndProcessings()) == count,
              "Failed binding must remove the added monitor");
        auto missing = create(engine, "ControlBinding { clamInControl: 'Source.Missing' }");
        errors.clear();
        check(!bindQmlInterface(missing.get(), network, errors), "Missing controls must fail gracefully");

        // Legacy bindings are owned by their UI, including ControlSource
        // senders; destroying that UI must not double-delete them at exit.
        network.AddProcessing("LegacySource", new CLAM::ControlSource);
        auto legacySlider = std::make_unique<QSlider>();
        legacySlider->setObjectName("ControlSource__LegacySource");
        errors.clear();
        CLAM::QtBinder::bindAllBinders(legacySlider.get(), network, errors);
        check(errors.isEmpty(), "Legacy ControlSource binding failed");
        legacySlider.reset();
        QWidget wrongProgressWidget;
        wrongProgressWidget.setObjectName("ProgressControl__Source");
        errors.clear();
        CLAM::QtBinder::bindAllBinders(&wrongProgressWidget, network, errors);
        check(!errors.isEmpty(), "A wrong legacy progress widget must report an error");

        // Exercise bundled layouts and real network port names without audio hardware.
        for (const QString &name : {QString("FilePlayer"), QString("tonalAnalysis-file")}) {
            CLAM::Network example;
            CLAM::XMLStorage::Restore(example, std::string(CLAM_SOURCE_DIR "/NetworkEditor/example-data/") +
                                                   name.toStdString() + ".clamnetwork");
            QQmlComponent component(
                &engine,
                QUrl::fromLocalFile(QString(CLAM_SOURCE_DIR "/NetworkEditor/example-data/") + name + ".qml"));
            while (component.isLoading())
                app.processEvents();
            if (component.isError())
                throw std::runtime_error(component.errorString().toStdString());
            std::unique_ptr<QObject> window(component.create());
            check(bool(window), "Example interface failed to load");
            errors.clear();
            check(bindQmlInterface(window.get(), example, errors), qPrintable(errors.join("\n")));
            events(app, 20);
        }
        std::cout << "QML loading, bindings, frame rendering and lifetime checks passed\n";
    } catch (const std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
