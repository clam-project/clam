#ifndef _PROTOTYPE_LOADER_HXX_
#define _PROTOTYPE_LOADER_HXX_

#include <QDialog>
#include <CLAM/NetworkPlayer.hxx>
#include <CLAM/PortMonitor.hxx>

#include <CLAM/AudioManager.hxx>
#include <CLAM/Network.hxx>

#include <iostream>
#include <list>
#include <string>
#include <memory>

class QPushButton;
class QQmlApplicationEngine;
class QQuickWindow;

namespace CLAM
{

class PrototypeBinder;

class PrototypeLoader : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool playing READ IsPlaying NOTIFY playbackChanged)
	Q_PROPERTY(bool paused READ IsPaused NOTIFY playbackChanged)
	Q_PROPERTY(bool stopped READ IsStopped NOTIFY playbackChanged)
	Q_PROPERTY(QString playbackStatus READ PlaybackStatus NOTIFY playbackChanged)
	Q_PROPERTY(QString backendName READ BackendName NOTIFY playbackChanged)
private:
	std::string _networkFile;
	std::string _backendName;
	Network _network;
	NetworkPlayer * _player;
	QWidget * _interface;
	std::unique_ptr<QQmlApplicationEngine> _qmlEngine;
	std::unique_ptr<QQuickWindow> _qmlHostWindow;
	QObject * _qmlRoot = nullptr;
	bool _useGui;
	QPushButton * _playButton;
	QPushButton * _pauseButton;
	QPushButton * _stopButton;

public:
	PrototypeLoader();
	~PrototypeLoader();
	void UseGUI(bool useIt) { _useGui=useIt; }
	bool LoadNetwork(std::string networkFile);
	bool ChooseBackend( std::list<std::string> backends, const std::string & name);
	bool LoadInterface(QString uiFile);
	
	bool ConnectUiWithNetwork();
	void Show();
	bool IsPlaying() const { return _network.IsPlaying(); }
	bool IsPaused() const { return _network.IsPaused(); }
	bool IsStopped() const { return _network.IsStopped(); }
	QString PlaybackStatus() const;
	QString BackendName() const { return QString::fromStdString(_backendName); }

signals:
	void playbackChanged();

public slots:
	void Start();
	void Pause();
	void Stop();

private:
	void reportError(const QString & title, const QString & message);
	void reportWarning(const QString & title, const QString & message);

	void UpdatePlayStatus();
	void ClearInterface();
private slots:
	void periodicPlaybackStatusUpdate();
};


} //end namespace CLAM

#endif

