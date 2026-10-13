#ifndef NETWORKEDITOR_BACKENDCONFIGDIALOG_HXX
#define NETWORKEDITOR_BACKENDCONFIGDIALOG_HXX

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QVBoxLayout>

/** Small editor for the NetworkEditor's realtime audio backend settings. */
class BackendConfigDialog : public QDialog
{
public:
	BackendConfigDialog(
		const QString & currentBackend,
		const QString & inputConnection,
		const QString & outputConnection,
		QWidget * parent = 0)
		: QDialog(parent)
		, _backend(new QComboBox(this))
		, _inputConnection(new QComboBox(this))
		, _outputConnection(new QComboBox(this))
		, _refreshConnections(new QPushButton(tr("Refresh"), this))
		, _connectionHelp(new QLabel(this))
	{
		setWindowTitle(tr("Audio Backend"));
		setModal(true);

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

		QFormLayout * form = new QFormLayout;
		form->addRow(tr("Backend:"), _backend);
		form->addRow(tr("Input / source:"), _inputConnection);
		form->addRow(tr("Output / sink:"), _outputConnection);
		form->addRow(tr("Available connections:"), _refreshConnections);

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

		QVBoxLayout * layout = new QVBoxLayout(this);
		layout->addLayout(form);
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
		const QString text = _inputConnection->currentText().trimmed();
		if (_inputConnection->currentIndex() == 0 &&
			(text.isEmpty() || text == tr("Default / automatic")))
			return QString();
		return _inputConnection->currentData().toString().isEmpty() ?
			text :
			_inputConnection->currentData().toString();
	}

	QString outputConnection() const
	{
		const QString text = _outputConnection->currentText().trimmed();
		if (_outputConnection->currentIndex() == 0 &&
			(text.isEmpty() || text == tr("Default / automatic")))
			return QString();
		return _outputConnection->currentData().toString().isEmpty() ?
			text :
			_outputConnection->currentData().toString();
	}

private:
	static QStringList runCommand(const QString & program, const QStringList & arguments)
	{
		QProcess process;
		process.start(program, arguments);
		if (!process.waitForFinished(1500) || process.exitStatus() != QProcess::NormalExit)
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
		const QStringList & values)
	{
		combo->clear();
		combo->addItem(tr("Default / automatic"), QString());
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
			const QStringList ports = runCommand(QStringLiteral("jack_lsp"), QStringList());
			setConnectionItems(_inputConnection, inputValue, ports);
			setConnectionItems(_outputConnection, outputValue, ports);
		}
	}

	void updateHelp()
	{
		const QString selected = backend();
		if (selected == QStringLiteral("PipeWire"))
		{
			_connectionHelp->setText(tr(
				"Enter PipeWire node names or object serials. Leave a field empty "
				"to connect to the session's default source or sink."));
		}
		else if (selected == QStringLiteral("JACK"))
		{
			_connectionHelp->setText(tr(
				"Enter JACK port-name patterns. Leave a field empty to keep "
				"manual connection behavior."));
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
};

#endif
