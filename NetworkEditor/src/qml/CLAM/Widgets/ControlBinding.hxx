// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef CLAM_QML_CONTROL_BINDING_HXX
#define CLAM_QML_CONTROL_BINDING_HXX
#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QVariant>
#include <QtQml/qqmlregistration.h>
#include <atomic>
#include <functional>
#include <memory>
namespace CLAM {
class Network;
class InControlBase;
} // namespace CLAM

class ClamControlBinding : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(ControlBinding)
    Q_PROPERTY(QString clamInControl MEMBER _inControl NOTIFY bindingChanged)
    Q_PROPERTY(QString clamOutControl MEMBER _outControl NOTIFY bindingChanged)
    Q_PROPERTY(QString clamProcessing MEMBER _processing NOTIFY bindingChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QVariant observedValue READ observedValue NOTIFY observedValueChanged)
  public:
    explicit ClamControlBinding(QObject *parent = nullptr);
    ~ClamControlBinding() override;
    QVariant value() const { return _value; }
    QVariant observedValue() const { return _observedValue; }
    void setValue(const QVariant &value);
    bool bind(CLAM::Network &network, QStringList &errors);
    Q_INVOKABLE void send(const QVariant &value);
    Q_INVOKABLE void configure();
    Q_INVOKABLE void chooseAudioFile();
    Q_INVOKABLE void sendSurface(double x, double y);
    Q_INVOKABLE void sendMidi(int note, bool pressed, int velocity = 90);
  signals:
    void bindingChanged();
    void valueChanged();
    void observedValueChanged();
    void errorOccurred(const QString &message);

  private:
    QString _inControl, _outControl, _processing;
    QVariant _value, _observedValue;
    CLAM::Network *_network = nullptr;
    std::function<void(const QVariant &)> _send;
    std::atomic<double> _receivedValue{0};
    std::atomic<bool> _hasReceived{false};
    std::unique_ptr<CLAM::InControlBase> _observer;
    bool _observingBool = false;
    QTimer _timer;
    void fail(const QString &message);
};
#endif
