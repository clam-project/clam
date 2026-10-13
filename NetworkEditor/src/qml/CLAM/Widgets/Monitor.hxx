// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef CLAM_QML_MONITOR_HXX
#define CLAM_QML_MONITOR_HXX

#include <CLAM/DataTypes.hxx>
#include <QImage>
#include <QQuickPaintedItem>
#include <QStringList>
#include <QTimer>
#include <QtQml/qqmlregistration.h>
#include <vector>

namespace CLAM {
class Network;
namespace VM {
class FloatArrayDataSource;
class SegmentationDataSource;
} // namespace VM
} // namespace CLAM
class PeakDataSource;

class ClamQuickMonitor : public QQuickPaintedItem {
    Q_OBJECT
    QML_NAMED_ELEMENT(Monitor)
    Q_PROPERTY(Type monitorType READ monitorType WRITE setMonitorType NOTIFY appearanceChanged)
    Q_PROPERTY(QString clamOutPort MEMBER _outPort NOTIFY bindingChanged)
    Q_PROPERTY(QColor lineColor MEMBER _lineColor NOTIFY appearanceChanged)
    Q_PROPERTY(QColor backgroundColor MEMBER _backgroundColor NOTIFY appearanceChanged)
    Q_PROPERTY(double lowerBound MEMBER _lowerBound NOTIFY appearanceChanged)
    Q_PROPERTY(double upperBound MEMBER _upperBound NOTIFY appearanceChanged)
    Q_PROPERTY(double timeSpan MEMBER _timeSpan NOTIFY appearanceChanged)
    Q_PROPERTY(int refreshInterval READ refreshInterval WRITE setRefreshInterval NOTIFY appearanceChanged)
    Q_PROPERTY(int sampleCount READ sampleCount NOTIFY frameChanged)
  public:
    enum Type {
        Oscilloscope,
        BufferOscilloscope,
        Vumeter,
        SpectrumView,
        PeakView,
        VectorView,
        HistogramView,
        LPModelView,
        MelSpectrumView,
        MelCepstrumView,
        Spectrogram,
        Tonnetz,
        KeySpace,
        ChordRanking,
        PolarChromaPeaks,
        Tunner,
        SegmentationView
    };
    Q_ENUM(Type)
    explicit ClamQuickMonitor(QQuickItem *parent = nullptr);
    Type monitorType() const { return _type; }
    void setMonitorType(Type type);
    int refreshInterval() const { return _timer.interval(); }
    void setRefreshInterval(int ms);
    int sampleCount() const { return int(_values.size()); }
    bool bind(CLAM::Network &network, QStringList &errors);
    void paint(QPainter *painter) override;
  public slots:
    void refresh();
  signals:
    void appearanceChanged();
    void bindingChanged();
    void frameChanged();

  private:
    Type _type = Oscilloscope;
    QString _outPort;
    QColor _lineColor = QColor("#52b7dc");
    QColor _backgroundColor = QColor("#15212c");
    double _lowerBound = -1;
    double _upperBound = 1;
    double _timeSpan = 10;
    QTimer _timer;
    CLAM::VM::FloatArrayDataSource *_arraySource = nullptr;
    PeakDataSource *_peakSource = nullptr;
    CLAM::VM::SegmentationDataSource *_segmentSource = nullptr;
    std::vector<CLAM::TData> _values, _positions;
    QStringList _labels;
    struct Segment {
        double onset, offset;
        QString label;
    };
    std::vector<Segment> _segments;
    double _currentTime = 0;
    double _sourceLower = -1, _sourceUpper = 1;
    QImage _history;
    template <typename Monitor> bool bindMonitor(CLAM::Network &, const std::string &, QStringList &);
};
#endif
