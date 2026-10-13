#include "PrototypeLoader.hxx"
#include <QUiLoader>
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QFile>
#include <QTimer>
#include <CLAM/XMLStorage.hxx>
#include <fstream>
#ifdef USE_JACK
#include <CLAM/JACKNetworkPlayer.hxx>
#endif
#ifdef USE_PORTAUDIO
#include <CLAM/PANetworkPlayer.hxx>
#endif


#include "QtBinder.hxx"
#include "QmlBindings.hxx"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWindow>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include "Oscilloscope.hxx"
#include "Vumeter.hxx"
#include "SpectrumView.hxx"
#include "PeakView.hxx"
#include "BufferOscilloscope.hxx"
#include "VectorView.hxx"
#include "HistogramView.hxx"
#include "Tonnetz.hxx"
#include "KeySpace.hxx"
#include "Spectrogram.hxx"
#include "PolarChromaPeaks.hxx"
#include "ChordRanking.hxx"
#include "LPModelView.hxx"
#include "MelSpectrumView.hxx"
#include "MelCepstrumView.hxx"
#include "SegmentationView.hxx"
#include "Tunner.hxx"
#include "PixmapWidgets.hxx"
#include "QSynthKnob.hxx"
#include "QFirstPerson.hxx"
#include "ControlSurfaceWidget.hxx"
#include "ProgressControlWidget.hxx"

namespace
{

static bool FileExists( const std::string filename )
{
	//Check for existence of XML Network file
	std::ifstream file( filename.c_str() );
	if( !file ) return false;
	return true;
}

}

namespace CLAM
{


PrototypeLoader::PrototypeLoader()
	: _player(0)
	, _interface(0)
	, _useGui(true)
	, _playButton(0)
	, _pauseButton(0)
	, _stopButton(0)
{
	periodicPlaybackStatusUpdate();
}

void PrototypeLoader::reportWarning(const QString & title, const QString & message)
{
	if (_useGui)
	{
		QMessageBox::warning(_interface, title, message);
		return;
	}
	std::cerr << "WARNING! " << title.toLocal8Bit().data() << std::endl;
	std::cout << message.toLocal8Bit().data() << std::endl;
}

void PrototypeLoader::reportError(const QString & title, const QString & message)
{
	if (_useGui)
	{
		QMessageBox::critical(_interface, title, message);
		return;
	}
	std::cerr << "ERROR! " << title.toLocal8Bit().data() << std::endl;
	std::cout << message.toLocal8Bit().data() << std::endl;
}

bool PrototypeLoader::LoadNetwork(std::string networkFile)
{
	_networkFile = networkFile;
	if (_networkFile.empty() && !_useGui)
    {
        reportError(tr("Error loading the network"), tr("Noninteractive mode requires a network file."));
        return false;
    }
    if (_networkFile.empty())
    {
		QString file = QFileDialog::getOpenFileName(0,
			"Choose a Network to run",
			QString(),
			"CLAM Network files (*.clamnetwork)");
		if (file.isEmpty()) return false;
		_networkFile=file.toLocal8Bit().constData();
	}
	QString errorMessage;
	try
   	{
		CLAM::XMLStorage::Restore(_network, _networkFile);
		return true;
	}
	catch (CLAM::XmlStorageErr & e)
	{
		errorMessage = FileExists(_networkFile) ?
			tr("<p>An error occurred while loading the network file %1.<p>"
				"<p><b>%2</b></p>").arg(_networkFile.c_str()).arg(e.what()) :
			tr("<p>Network file '%1' not found.</p>").arg(_networkFile.c_str());
	}
	reportError(
		tr("Error loading the network"),
		errorMessage);
	_network.Clear();
	return false;
}

PrototypeLoader::~PrototypeLoader()
{
    _network.Stop();
    ClearInterface();
}

void PrototypeLoader::ClearInterface()
{
    // Stop audio before destroying control observers and monitor sources.
    _network.Stop();
    _playButton = _pauseButton = _stopButton = nullptr;
    _qmlRoot = nullptr;
    _qmlEngine.reset();
    _qmlHostWindow.reset();
    delete _interface;
    _interface = nullptr;
}
void PrototypeLoader::Show()
{
    if (_interface) _interface->show();
    if (auto * window = qobject_cast<QQuickWindow *>(_qmlRoot)) window->show();
    if (_qmlHostWindow) _qmlHostWindow->show();
}


CLAM::NetworkPlayer * tryNetworkPlayer(const std::string & backend, const std::string & name)
{	
#ifdef USE_JACK		
	if (backend=="jack")
	{
		CLAM::JACKNetworkPlayer * player = new CLAM::JACKNetworkPlayer(name);
		if ( player->IsWorking()) return player;
		delete player;
		return 0;
	}
#endif
#ifdef USE_PORTAUDIO
	if (backend=="portaudio")
	{
		return new CLAM::PANetworkPlayer;
	}
#endif
	return 0;
}

bool PrototypeLoader::ChooseBackend( std::list<std::string> backends, const std::string & name)
{
	// If no backends specified, do the standard backend waterfall
	if (backends.empty())
	{
		backends.push_back("jack");
		backends.push_back("portaudio");
	}
	for (std::list<std::string>::iterator it = backends.begin();
			it!=backends.end();
			it++)
	{
		_player = tryNetworkPlayer(*it, name);
		if ( !_player)
		{
			std::cerr << "Backend '" << *it << "' unavailable." << std::endl;
			continue;
		}
		_network.SetPlayer(_player);
		_backendName = *it;
		std::cout << "Backend '" << *it << "' selected." << std::endl;
		return true;
	}
	reportError(
		tr("Error choosing a backend"),
		tr("No audio backend was available."));
	return false;
}

// Construct the legacy CLAM widgets directly. A build-tree Prototyper
// should not depend on finding an installed Qt Designer plugin.
class LegacyUiLoader : public QUiLoader
{
protected:
    QWidget * createWidget(const QString & type, QWidget * parent, const QString & name) override
    {
        QWidget * widget = nullptr;
#define CLAM_UI_WIDGET(className, Class) if (type == className) widget = new Class(parent); else
        CLAM_UI_WIDGET("Oscilloscope", Oscilloscope)
        CLAM_UI_WIDGET("Vumeter", Vumeter)
        CLAM_UI_WIDGET("SpectrumView", SpectrumView)
        CLAM_UI_WIDGET("PeakView", PeakView)
        CLAM_UI_WIDGET("BufferOscilloscope", BufferOscilloscope)
        CLAM_UI_WIDGET("CLAM::VM::VectorView", CLAM::VM::VectorView)
        CLAM_UI_WIDGET("CLAM::VM::HistogramView", CLAM::VM::HistogramView)
        CLAM_UI_WIDGET("CLAM::VM::Tonnetz", CLAM::VM::Tonnetz)
        CLAM_UI_WIDGET("CLAM::VM::KeySpace", CLAM::VM::KeySpace)
        CLAM_UI_WIDGET("CLAM::VM::Spectrogram", CLAM::VM::Spectrogram)
        CLAM_UI_WIDGET("PolarChromaPeaks", PolarChromaPeaks)
        CLAM_UI_WIDGET("CLAM::VM::ChordRanking", CLAM::VM::ChordRanking)
        CLAM_UI_WIDGET("CLAM::VM::LPModelView", CLAM::VM::LPModelView)
        CLAM_UI_WIDGET("CLAM::VM::MelSpectrumView", CLAM::VM::MelSpectrumView)
        CLAM_UI_WIDGET("CLAM::VM::MelCepstrumView", CLAM::VM::MelCepstrumView)
        CLAM_UI_WIDGET("SegmentationView", SegmentationView)
        CLAM_UI_WIDGET("Tunner", Tunner)
        CLAM_UI_WIDGET("QSynthKnob", QSynthKnob)
        CLAM_UI_WIDGET("QFirstPerson", QFirstPerson)
        CLAM_UI_WIDGET("QPixmapButton", QPixmapButton)
        if (type == "QPixmapRange") widget = new QPixmapRange(":/pkwidgets/thinMixerSlider_", parent);
        else if (type == "ControlSurfaceWidget") widget = new ControlSurfaceWidget(nullptr, parent);
        else if (type == "ProgressControlWidget") widget = new ProgressControlWidget(nullptr, parent);
        else widget = QUiLoader::createWidget(type, parent, name);
#undef CLAM_UI_WIDGET
        if (widget) widget->setObjectName(name);
        return widget;
    }
};

static QWidget * DoLoadInterface(const QString & uiFile)
{
    QFile file(uiFile);
    if (!file.open(QFile::ReadOnly)) return nullptr;
    LegacyUiLoader loader;
    QWidget * userInterface = loader.load(&file);
    if (userInterface && userInterface->windowIcon().isNull())
        userInterface->setWindowIcon(QIcon(":/icons/images/Prototyper-icon.svg"));
    if (!userInterface) qWarning().noquote() << loader.errorString();
    return userInterface;
}

bool PrototypeLoader::LoadInterface(QString uiFile)
{
    ClearInterface();
    if (uiFile.isEmpty()) {
        const QFileInfo network(QString::fromStdString(_networkFile));
        const QString base = network.dir().filePath(network.completeBaseName());
        // Prefer QML when both versions of an interface are present.
        uiFile = QFileInfo::exists(base + ".qml") ? base + ".qml" : base + ".ui";
        if (!QFileInfo::exists(uiFile)) uiFile = ":/interfaces/prototyper/DefaultInterface.qml";
    }
    if (uiFile.endsWith(".qml", Qt::CaseInsensitive)) {
        _qmlEngine = std::make_unique<QQmlApplicationEngine>();
        _qmlEngine->addImportPath("qrc:/qt/qml");
        const QDir executable(QCoreApplication::applicationDirPath());
        _qmlEngine->addImportPath(executable.filePath("../qml"));
        _qmlEngine->addImportPath(executable.filePath("../" CLAM_QML_INSTALL_DIR));
        _qmlEngine->rootContext()->setContextProperty("prototyper", this);
        // Root windows become visible only after the network bindings succeed.
        _qmlEngine->load(uiFile.startsWith(":/") ? QUrl("qrc" + uiFile)
            : QUrl::fromLocalFile(QFileInfo(uiFile).absoluteFilePath()));
        if (_qmlEngine->rootObjects().isEmpty()) {
            reportError(tr("Error loading the QML interface"), tr("Could not load '%1'. See QML diagnostics on stderr.").arg(uiFile));
            _qmlEngine.reset();
            return false;
        }
        _qmlRoot = _qmlEngine->rootObjects().first();
        if (auto * item = qobject_cast<QQuickItem *>(_qmlRoot)) {
            _qmlHostWindow = std::make_unique<QQuickWindow>();
            _qmlHostWindow->resize(qMax(1, int(item->width() > 0 ? item->width() : item->implicitWidth())),
                qMax(1, int(item->height() > 0 ? item->height() : item->implicitHeight())));
            item->setParentItem(_qmlHostWindow->contentItem());
            auto resizeRoot = [this, item] {
                item->setWidth(_qmlHostWindow->width());
                item->setHeight(_qmlHostWindow->height());
            };
            connect(_qmlHostWindow.get(), &QQuickWindow::widthChanged, this, resizeRoot);
            connect(_qmlHostWindow.get(), &QQuickWindow::heightChanged, this, resizeRoot);
            resizeRoot();
        } else if (!qobject_cast<QQuickWindow *>(_qmlRoot)) {
            reportError(tr("Invalid QML root"), tr("Use a Window, ApplicationWindow or Item as the root."));
            ClearInterface();
            return false;
        }
        return true;
    }
    _interface = DoLoadInterface(uiFile);
    if (_interface) return true;
    reportError(tr("Error loading the interface"), tr("Could not load interface '%1'.").arg(uiFile));
    return false;
}

bool PrototypeLoader::ConnectUiWithNetwork()
{
	CLAM_ASSERT( _player, "Connecting interface without having chosen a backend");
	
	QStringList errors;
    if (_qmlRoot) {
        if (!bindQmlInterface(_qmlRoot, _network, errors)) {
            reportError(tr("Interface Binding Errors"), errors.join("\n"));
            return false;
        }
        UpdatePlayStatus();
        return true;
    }
    if (!_interface) return false;
    QtBinder::bindAllBinders(_interface, _network, errors);
	if (!errors.empty()) {
        reportError(tr("Interface Binding Errors"), errors.join("\n"));
        return false;
    }
		

	_playButton = _interface->findChild<QPushButton*>("PlayButton");
	_pauseButton = _interface->findChild<QPushButton*>("PauseButton");
	_stopButton = _interface->findChild<QPushButton*>("StopButton");
	if (_playButton) connect(_playButton, SIGNAL(clicked()), this, SLOT(Start()));  
	if (_pauseButton) connect(_pauseButton, SIGNAL(clicked()), this, SLOT(Pause()));  
	if (_stopButton) connect(_stopButton, SIGNAL(clicked()), this, SLOT(Stop()));

	QLabel * backendIndicator = _interface->findChild<QLabel*>("BackendIndicator");
	if (backendIndicator)
	{
		QString backendIcon = ":/icons/images/";
		backendIcon+=_backendName.c_str();
		backendIcon+="logo-mini.png";
		backendIndicator->setPixmap(backendIcon);
		backendIndicator->setToolTip(
			tr("<p>Using '%1' backend</p><p><img src='%2'/></p>")
			.arg(_backendName.c_str())
			.arg(backendIcon));
	}
    UpdatePlayStatus();
    return true;
}


void PrototypeLoader::Start()
{
	UpdatePlayStatus();
	if (  !_player )
	{
		reportError(
			tr("Unable to play the network"), 
			tr("<p><b>Audio output unavailable or busy.</b></p>"));
		return;
	}
	
	if (  _network.IsEmpty() )
	{
		reportError(
			tr("Unable to play the network"), 
			tr("<p><b>A network without processings is not playable.</b></p>"));
		return;
	}
	if (_network.HasMisconfiguredProcessings())
	{
		reportError(
			tr("Unable to play the network"), 
			tr("<p><b>Not all the processings are properly configured.</b></p><pre>%1</pre>"
			).arg(_network.GetConfigurationErrors().c_str()));
		return;
	}
	// TODO: Activate this once it works
	if ( false && _network.HasUnconnectedInPorts() )
	{
		reportError(
			tr("Unable to play the network"), 
			tr("<p><b>The network has some in ports which are not connected.</b></p>"
			"<p>All in ports must be feeded in order to play the network</p>"
			));
		return;
	}
	std::cout << "Starting..." << std::endl;
	_network.Start();
	UpdatePlayStatus();
}

void PrototypeLoader::Stop()
{
	std::cout << "Stopping..." << std::endl;
	_network.Stop();
	UpdatePlayStatus();
}

void PrototypeLoader::Pause()
{
	std::cout << "Pausing..." << std::endl;
	_network.Pause();
	UpdatePlayStatus();
}

void PrototypeLoader::periodicPlaybackStatusUpdate()
{
	UpdatePlayStatus();
	QTimer::singleShot(500, this, SLOT(periodicPlaybackStatusUpdate()));
}

QString PrototypeLoader::PlaybackStatus() const
{
    if (_network.IsPlaying()) return tr("Playing");
    if (_network.IsPaused()) return tr("Paused");
    return tr("Stopped");
}

void PrototypeLoader::UpdatePlayStatus()
{
    emit playbackChanged();
	if (!_interface) return;
	QLabel * playbackIndicator = _interface->findChild<QLabel*>("PlaybackIndicator");
	if (playbackIndicator)
	{
		if ( _network.IsPlaying())
			playbackIndicator->setText(tr("<p style='color:green'>Playing...</p>"));
		else if (_network.IsPaused())
			playbackIndicator->setText(tr("<p style='color:orange;text-decoration:blink'>Paused</p>"));
		else
			playbackIndicator->setText(tr("<p style='color:red'>Stopped</p>"));
	}
	if (_playButton) _playButton->setEnabled(not _network.IsPlaying());
	if (_pauseButton) _pauseButton->setEnabled(_network.IsPlaying());
	if (_stopButton) _stopButton->setEnabled(not _network.IsStopped());
}


} //end namespace CLAM

