// SPDX-License-Identifier: GPL-2.0-or-later
#include "ControlBinding.hxx"
#include "BoolControlSender.hxx"
#include "ConfigurationQtBinder.hxx"
#include "ControlPiano.hxx"
#include "ControlSurface.hxx"
#include "QtBinder.hxx"
#include <CLAM/ControlSource.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/MonoAudioFileReaderConfig.hxx>
#include <CLAM/Network.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/OutControlSender.hxx>
#include <QDebug>
#include <QFileDialog>
#include <cmath>

namespace {
// Audio callbacks publish scalars without calling QML or racing a QVariant.
template <typename T> class AtomicObserver : public CLAM::InControl<T> {
    std::atomic<double> &_value;
    std::atomic<bool> &_received;

  public:
    AtomicObserver(std::atomic<double> &value, std::atomic<bool> &received)
        : CLAM::InControl<T>("QmlObserver"), _value(value), _received(received) {}
    void DoControl(const T &value) override {
        _value.store(double(value), std::memory_order_relaxed);
        _received.store(true, std::memory_order_release);
    }
};
} // namespace

ClamControlBinding::ClamControlBinding(QObject *parent) : QObject(parent) {
    _timer.setInterval(50);
    connect(&_timer, &QTimer::timeout, this, [this] {
        if (!_hasReceived.load(std::memory_order_acquire))
            return;
        const double value = _receivedValue.load(std::memory_order_relaxed);
        const QVariant next = _observingBool ? QVariant(bool(value)) : QVariant(value);
        if (_observedValue == next)
            return;
        _observedValue = next;
        emit observedValueChanged();
    });
}
ClamControlBinding::~ClamControlBinding() = default;

void ClamControlBinding::setValue(const QVariant &value) {
    if (_value == value)
        return;
    _value = value;
    emit valueChanged();
    send(value);
}

bool ClamControlBinding::bind(CLAM::Network &network, QStringList &errors) {
    if (_network)
        return true;
    std::function<void(const QVariant &)> send;
    if (!_inControl.isEmpty()) {
        const std::string name = _inControl.toStdString();
        if (name.find('.') == std::string::npos)
            return CLAM::QtBinder::error(
                errors, tr("clamInControl must be 'Processing.Control': %1").arg(_inControl));
        if (CLAM::QtBinder::missingInControl(name, network, errors))
            return false;
        auto &input = network.GetInControlByCompleteName(name);
        if (input.GetTypeId() != typeid(float) && input.GetTypeId() != typeid(bool) &&
            input.GetTypeId() != typeid(int) && input.GetTypeId() != typeid(std::string))
            return CLAM::QtBinder::error(errors, tr("Unsupported input control type: %1").arg(_inControl));
        // Configuration dialogs can rebuild a processing's controls. Resolve
        // the connector again for each send instead of retaining a freed pointer.
        send = [this, &network, name](const QVariant &value) {
            QStringList errors;
            if (CLAM::QtBinder::missingInControl(name, network, errors)) {
                fail(errors.join("\n"));
                return;
            }
            auto &input = network.GetInControlByCompleteName(name);
            if (auto *control = dynamic_cast<CLAM::InControl<float> *>(&input)) {
                const float number = value.toFloat();
                if (std::isfinite(number))
                    control->DoControl(number);
            } else if (auto *control = dynamic_cast<CLAM::InControl<bool> *>(&input))
                control->DoControl(value.toBool());
            else if (auto *control = dynamic_cast<CLAM::InControl<int> *>(&input))
                control->DoControl(value.toInt());
            else if (auto *control = dynamic_cast<CLAM::InControl<std::string> *>(&input))
                control->DoControl(value.toString().toStdString());
            else
                fail(tr("Unsupported input control type: %1").arg(QString::fromStdString(name)));
        };
    }
    if (!_processing.isEmpty()) {
        if (CLAM::QtBinder::missingProcessing(_processing.toStdString(), network, errors))
            return false;
        auto &processing = network.GetProcessing(_processing.toStdString());
        if (_inControl.isEmpty() && _value.isValid()) {
            if (auto *source = dynamic_cast<CLAM::ControlSource *>(&processing))
                send = [source](const QVariant &v) { source->Do(v.toFloat()); };
            else if (auto *source = dynamic_cast<CLAM::OutControlSender *>(&processing))
                send = [source](const QVariant &v) { source->SendControl(v.toFloat()); };
            else if (auto *source = dynamic_cast<BoolControlSender *>(&processing)) {
                if (!source->GetNOutControls())
                    return CLAM::QtBinder::error(errors, tr("BoolControlSender has no outputs."));
                send = [source](const QVariant &v) { source->SendControl(0, v.toBool()); };
            } else
                return CLAM::QtBinder::error(errors, tr("%1 cannot send scalar controls.").arg(_processing));
        }
    }
    if (!_outControl.isEmpty()) {
        const std::string name = _outControl.toStdString();
        if (name.find('.') == std::string::npos)
            return CLAM::QtBinder::error(
                errors, tr("clamOutControl must be 'Processing.Control': %1").arg(_outControl));
        const auto processingName = network.GetProcessingIdentifier(name);
        if (CLAM::QtBinder::missingProcessing(processingName, network, errors))
            return false;
        if (!network.GetProcessing(processingName).HasOutControl(network.GetConnectorIdentifier(name)))
            return CLAM::QtBinder::error(errors, tr("Output control '%1' does not exist.").arg(_outControl));
        auto &output = network.GetOutControlByCompleteName(name);
        if (output.GetTypeId() == typeid(float))
            _observer = std::make_unique<AtomicObserver<float>>(_receivedValue, _hasReceived);
        else if (output.GetTypeId() == typeid(bool)) {
            _observer = std::make_unique<AtomicObserver<bool>>(_receivedValue, _hasReceived);
            _observingBool = true;
        } else if (output.GetTypeId() == typeid(int))
            _observer = std::make_unique<AtomicObserver<int>>(_receivedValue, _hasReceived);
        else
            return CLAM::QtBinder::error(errors, tr("Unsupported output control type: %1").arg(_outControl));
        output.AddLink(*_observer);
        _timer.start();
    }
    _network = &network;
    _send = std::move(send);
    if (_value.isValid())
        this->send(_value);
    return true;
}

void ClamControlBinding::send(const QVariant &value) {
    if (_send && value.isValid())
        _send(value);
}
void ClamControlBinding::fail(const QString &message) {
    qWarning().noquote() << message;
    emit errorOccurred(message);
}
void ClamControlBinding::configure() {
    if (!_network || _processing.isEmpty())
        return;
    ConfigurationLauncher launcher(*_network, _processing.toStdString());
    launcher.launch();
}
void ClamControlBinding::chooseAudioFile() {
    if (!_network || _processing.isEmpty())
        return;
    auto &processing = _network->GetProcessing(_processing.toStdString());
    const auto *previous = dynamic_cast<const CLAM::MonoAudioFileReaderConfig *>(&processing.GetConfig());
    if (!previous) {
        fail(tr("%1 is not a MonoAudioFileReader.").arg(_processing));
        return;
    }
    const QString path = QFileDialog::getOpenFileName(nullptr, tr("Choose an audio file"),
                                                      QString::fromStdString(previous->GetSourceFile()),
                                                      tr("Audio files (*.wav *.ogg *.mp3)"));
    if (path.isEmpty())
        return;
    auto config = *previous;
    config.SetSourceFile(path.toStdString());
    const bool playing = _network->IsPlaying(), paused = _network->IsPaused();
    _network->Stop();
    if (!processing.Configure(config)) {
        fail(QString::fromStdString(processing.GetConfigErrorMessage()));
        return;
    }
    if (playing || paused)
        _network->Start();
    if (paused)
        _network->Pause();
}
void ClamControlBinding::sendSurface(double x, double y) {
    if (!_network || _processing.isEmpty() || !std::isfinite(x) || !std::isfinite(y))
        return;
    auto *surface = dynamic_cast<CLAM::ControlSurface *>(&_network->GetProcessing(_processing.toStdString()));
    if (!surface) {
        fail(tr("%1 is not a ControlSurface.").arg(_processing));
        return;
    }
    surface->SendControl(x, y);
}
void ClamControlBinding::sendMidi(int note, bool pressed, int velocity) {
    if (!_network || _processing.isEmpty())
        return;
    auto *piano = dynamic_cast<CLAM::ControlPiano *>(&_network->GetProcessing(_processing.toStdString()));
    if (!piano) {
        fail(tr("%1 is not a ControlPiano.").arg(_processing));
        return;
    }
    if (note < 0 || note > 127 || velocity < 0 || velocity > 127)
        return;
    MIDI::Message message(pressed ? 144 : 128, note, pressed ? velocity : 0, 0);
    piano->SendMIDIMessage(message);
}
