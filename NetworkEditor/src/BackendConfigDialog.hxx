#ifndef NETWORKEDITOR_BACKENDCONFIGDIALOG_HXX
#define NETWORKEDITOR_BACKENDCONFIGDIALOG_HXX

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

/** Small editor for the NetworkEditor's realtime audio backend settings. */
class BackendConfigDialog : public QDialog
{
public:
	BackendConfigDialog(
		const QString & currentBackend,
		const QString & inputConnection,
		const QString & outputConnection,
		QWidget * parent = 0,
		const QStringList & sourceChannels = QStringList(),
		const QStringList & sinkChannels = QStringList(),
		const QMap<QString, QString> & sourceConnections = QMap<QString, QString>(),
		const QMap<QString, QString> & sinkConnections = QMap<QString, QString>())
		: QDialog(parent)
		, _backend(new QComboBox(this))
		, _inputConnection(new QComboBox(this))
		, _outputConnection(new QComboBox(this))
		, _refreshConnections(new QPushButton(tr("Refresh"), this))
		, _connectionHelp(new QLabel(this))
		, _jackRouting(new QComboBox(this))
		, _patternConnections(new QWidget(this))
		, _jackConnections(new QScrollArea(this))
		, _form(new QFormLayout)
		, _sourceChannels(sourceChannels)
		, _sinkChannels(sinkChannels)
	{
		setWindowTitle(tr("Audio Backend"));
		setModal(true);
		_backend->setObjectName(QStringLiteral("backend"));
		_inputConnection->setObjectName(QStringLiteral("inputPattern"));
		_outputConnection->setObjectName(QStringLiteral("outputPattern"));
		_jackRouting->setObjectName(QStringLiteral("jackRouting"));
		_refreshConnections->setObjectName(QStringLiteral("refreshConnections"));

		_backend->addItem(tr("Automatic"), QStringLiteral("Auto"));
#ifdef USE_PIPEWIRE
		_backend->addItem(tr("PipeWire"), QStringLiteral("PipeWire"));
#endif
#ifdef USE_JACK
		_backend->addItem(tr("JACK"), QStringLiteral("JACK"));
#endif
#ifdef USE_PORTAUDIO
		_backend->addItem(tr("PortAudio"), QStringLiteral("PortAudio"));
#endif
		int currentIndex = _backend->findData(currentBackend);
		if (currentIndex >= 0)
			_backend->setCurrentIndex(currentIndex);

		_inputConnection->setEditable(true);
		_outputConnection->setEditable(true);
		_inputConnection->setInsertPolicy(QComboBox::NoInsert);
		_outputConnection->setInsertPolicy(QComboBox::NoInsert);
		_connectionHelp->setWordWrap(true);
		_jackRouting->addItem(tr("Per-channel ports"));
		_jackRouting->addItem(tr("Port-name patterns"));
		if (sourceConnections.isEmpty() && sinkConnections.isEmpty() &&
			(!inputConnection.isEmpty() || !outputConnection.isEmpty()))
			_jackRouting->setCurrentIndex(1);

		_form->addRow(tr("Backend:"), _backend);
		_form->addRow(tr("JACK routing:"), _jackRouting);
		QFormLayout * patterns = new QFormLayout(_patternConnections);
		patterns->setContentsMargins(0, 0, 0, 0);
		patterns->addRow(tr("Input / source:"), _inputConnection);
		patterns->addRow(tr("Output / sink:"), _outputConnection);
		QWidget * channels = new QWidget(this);
		QVBoxLayout * channelLayout = new QVBoxLayout(channels);
		addChannelSelectors(channelLayout, tr("Capture / network inputs"),
			sourceChannels, sourceConnections, _sourceSelectors, QStringLiteral("capture"));
		addChannelSelectors(channelLayout, tr("Playback / network outputs"),
			sinkChannels, sinkConnections, _sinkSelectors, QStringLiteral("playback"));
		_jackConnections->setWidget(channels);
		_jackConnections->setWidgetResizable(true);
		_jackConnections->setMinimumSize(460, 240);

		QDialogButtonBox * buttons = new QDialogButtonBox(
			QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
			Qt::Horizontal,
			this);
		connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
		connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
		connect(_backend, &QComboBox::currentTextChanged, this,
			[this](const QString &) { updateHelp(); refreshConnections(); });
		connect(_refreshConnections, &QPushButton::clicked, this,
			[this]() { refreshConnections(); });
		connect(_jackRouting, &QComboBox::currentIndexChanged, this,
			[this](int) { updateHelp(); });

		QVBoxLayout * layout = new QVBoxLayout(this);
		layout->addLayout(_form);
		layout->addWidget(_patternConnections);
		layout->addWidget(_jackConnections);
		layout->addWidget(_refreshConnections);
		layout->addWidget(_connectionHelp);
		layout->addWidget(buttons);
		updateHelp();
		refreshConnections(inputConnection, outputConnection);
	}

	QString backend() const
	{
		return _backend->currentData().toString();
	}

	QString inputConnection() const
	{
		return connectionValue(_inputConnection);
	}

	QString outputConnection() const
	{
		return connectionValue(_outputConnection);
	}

	bool usesJackPortConnections() const
	{
		return backend() == QStringLiteral("JACK") && _jackRouting->currentIndex() == 0;
	}

	QMap<QString, QString> sourceConnections() const
	{
		return channelConnections(_sourceChannels, _sourceSelectors);
	}

	QMap<QString, QString> sinkConnections() const
	{
		return channelConnections(_sinkChannels, _sinkSelectors);
	}

private:
	static QString connectionValue(const QComboBox * combo)
	{
		const int index = combo->currentIndex();
		if (index >= 0 && combo->currentText() == combo->itemText(index))
			return combo->currentData().toString();
		return combo->currentText().trimmed();
	}

	static QMap<QString, QString> channelConnections(const QStringList & names,
		const QList<QComboBox *> & selectors)
	{
		QMap<QString, QString> result;
		for (int i = 0; i < names.size(); ++i)
			result.insert(names[i], connectionValue(selectors[i]));
		return result;
	}

	void addChannelSelectors(QVBoxLayout * layout, const QString & title,
		const QStringList & names, const QMap<QString, QString> & connections,
		QList<QComboBox *> & selectors, const QString & prefix)
	{
		QGroupBox * group = new QGroupBox(title, this);
		QFormLayout * form = new QFormLayout(group);
		if (names.isEmpty())
			form->addRow(new QLabel(tr("No audio channels in the current network."), group));
		for (int i = 0; i < names.size(); ++i)
		{
			QComboBox * combo = new QComboBox(group);
			combo->setObjectName(prefix + QString::number(i + 1));
			combo->setEditable(true);
			combo->setInsertPolicy(QComboBox::NoInsert);
			setConnectionItems(combo, connections.value(names[i]), QStringList(), tr("Manual / unconnected"));
			selectors << combo;
			form->addRow(tr("Channel %1 (%2):").arg(i + 1).arg(names[i]), combo);
		}
		layout->addWidget(group);
	}

	static QStringList runCommand(const QString & program, const QStringList & arguments)
	{
		QProcess process;
		process.start(program, arguments);
		if (!process.waitForFinished(1500) || process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0)
			return QStringList();
		return QString::fromLocal8Bit(process.readAllStandardOutput())
			.split('\n', Qt::SkipEmptyParts);
	}

	static QStringList pipeWireNodes(const QString & mediaClass)
	{
		const QStringList lines = runCommand(QStringLiteral("pw-cli"),
			QStringList() << QStringLiteral("ls") << QStringLiteral("Node"));
		QStringList result;
		QString nodeName;
		QString description;
		QString currentClass;
		for (const QString & rawLine : lines)
		{
			const QString line = rawLine.trimmed();
			if (line.startsWith(QStringLiteral("id ")) && !nodeName.isEmpty())
			{
				if (currentClass == mediaClass ||
					currentClass == QStringLiteral("Audio/Duplex"))
					result << (description.isEmpty() ? nodeName :
						description + QStringLiteral(" [") + nodeName + QStringLiteral("]"))
						+ QStringLiteral("\t") + nodeName;
				nodeName.clear();
				description.clear();
				currentClass.clear();
			}
			if (line.startsWith(QStringLiteral("node.name = ")))
				nodeName = unquote(line.section('=', 1).trimmed());
			else if (line.startsWith(QStringLiteral("node.description = ")))
				description = unquote(line.section('=', 1).trimmed());
			else if (line.startsWith(QStringLiteral("media.class = ")))
				currentClass = unquote(line.section('=', 1).trimmed());
		}
		if (!nodeName.isEmpty() && (currentClass == mediaClass ||
			currentClass == QStringLiteral("Audio/Duplex")))
			result << (description.isEmpty() ? nodeName :
				description + QStringLiteral(" [") + nodeName + QStringLiteral("]"))
				+ QStringLiteral("\t") + nodeName;
		return result;
	}

	static QString unquote(QString value)
	{
		if (value.size() >= 2 && value.startsWith('"') && value.endsWith('"'))
			value = value.mid(1, value.size() - 2);
		return value;
	}

	void setConnectionItems(QComboBox * combo, const QString & current,
		const QStringList & values, const QString & emptyLabel = QString())
	{
		combo->clear();
		combo->addItem(emptyLabel.isEmpty() ? tr("Default / automatic") : emptyLabel, QString());
		for (const QString & value : values)
		{
			const QStringList parts = value.split('\t');
			if (parts.size() == 2)
				combo->addItem(parts[0], parts[1]);
			else
				combo->addItem(value, value);
		}
		const int currentIndex = combo->findData(current);
		if (currentIndex >= 0)
			combo->setCurrentIndex(currentIndex);
		else
		{
			combo->setCurrentIndex(0);
			combo->setEditText(current);
		}
	}

	static QPair<QStringList, QStringList> jackPorts()
	{
		const QStringList lines = runCommand(QStringLiteral("jack_lsp"),
			QStringList() << QStringLiteral("-p") << QStringLiteral("-t"));
		QPair<QStringList, QStringList> result;
		QString name;
		bool output = false, input = false, audio = false;
		auto append = [&]() {
			if (name.isEmpty() || !audio) return;
			if (output) result.first << name;
			if (input) result.second << name;
		};
		for (const QString & raw : lines)
		{
			const QString line = raw.trimmed();
			if (!raw.isEmpty() && !raw[0].isSpace())
			{
				append();
				name = line;
				output = input = audio = false;
			}
			else if (line.startsWith(QStringLiteral("properties:")))
			{
				const QStringList flags = line.mid(11).split(',');
				for (const QString & flag : flags)
				{
					output |= flag.trimmed() == QStringLiteral("output");
					input |= flag.trimmed() == QStringLiteral("input");
				}
			}
			else if (line == QStringLiteral("32 bit float mono audio"))
				audio = true;
		}
		append();
		return result;
	}

	void refreshConnections(
		const QString & input = QString(),
		const QString & output = QString())
	{
		const QString inputValue = input.isNull() ? inputConnection() : input;
		const QString outputValue = output.isNull() ? outputConnection() : output;
		const QString selected = backend();
		if (selected == QStringLiteral("PipeWire") || selected == QStringLiteral("Auto"))
		{
			setConnectionItems(_inputConnection, inputValue,
				pipeWireNodes(QStringLiteral("Audio/Source")));
			setConnectionItems(_outputConnection, outputValue,
				pipeWireNodes(QStringLiteral("Audio/Sink")));
		}
		else if (selected == QStringLiteral("JACK"))
		{
			const auto ports = jackPorts();
			setConnectionItems(_inputConnection, inputValue, ports.first, tr("Manual / unconnected"));
			setConnectionItems(_outputConnection, outputValue, ports.second, tr("Manual / unconnected"));
			for (QComboBox * combo : _sourceSelectors)
				setConnectionItems(combo, connectionValue(combo), ports.first, tr("Manual / unconnected"));
			for (QComboBox * combo : _sinkSelectors)
				setConnectionItems(combo, connectionValue(combo), ports.second, tr("Manual / unconnected"));
		}
	}

	void updateHelp()
	{
		const QString selected = backend();
		const bool jack = selected == QStringLiteral("JACK");
		_form->setRowVisible(_jackRouting, jack);
		_jackConnections->setVisible(usesJackPortConnections());
		_patternConnections->setVisible(!usesJackPortConnections());
		if (selected == QStringLiteral("PipeWire"))
		{
			_connectionHelp->setText(tr(
				"Enter PipeWire node names or object serials. Leave a field empty "
				"to connect to the session's default source or sink."));
		}
		else if (selected == QStringLiteral("JACK"))
		{
			_connectionHelp->setText(usesJackPortConnections() ? tr(
				"Choose an exact JACK audio port for each network channel. Capture uses "
				"JACK output ports; playback uses JACK input ports. Leave a channel "
				"manual to connect it externally.") : tr(
				"Enter JACK port-name patterns matching all desired channels, in JACK's "
				"port order. Leave a field empty to keep that direction manually connected."));
		}
		else
		{
			_connectionHelp->setText(tr(
				"Connection targets apply to PipeWire and JACK. PortAudio uses "
				"the system default devices."));
		}
	}

	QComboBox * _backend;
	QComboBox * _inputConnection;
	QComboBox * _outputConnection;
	QPushButton * _refreshConnections;
	QLabel * _connectionHelp;
	QComboBox * _jackRouting;
	QWidget * _patternConnections;
	QScrollArea * _jackConnections;
	QFormLayout * _form;
	QStringList _sourceChannels;
	QStringList _sinkChannels;
	QList<QComboBox *> _sourceSelectors;
	QList<QComboBox *> _sinkSelectors;
};

#endif
