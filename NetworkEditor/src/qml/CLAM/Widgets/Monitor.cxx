// SPDX-License-Identifier: GPL-2.0-or-later
#include "Monitor.hxx"
#include "BufferOscilloscopeMonitor.hxx"
#include "ChordRankingMonitor.hxx"
#include "HistogramViewMonitor.hxx"
#include "KeySpaceMonitor.hxx"
#include "LPModelViewMonitor.hxx"
#include "MelCepstrumViewMonitor.hxx"
#include "MelSpectrumViewMonitor.hxx"
#include "OscilloscopeMonitor.hxx"
#include "PeakViewMonitor.hxx"
#include "PolarChromaPeaksMonitor.hxx"
#include "QtBinder.hxx"
#include "SegmentationViewMonitor.hxx"
#include "SpectrogramMonitor.hxx"
#include "SpectrumViewMonitor.hxx"
#include "TonnetzMonitor.hxx"
#include "TunnerMonitor.hxx"
#include "VectorViewMonitor.hxx"
#include "VumeterMonitor.hxx"
#include <CLAM/Network.hxx>
#include <QPainter>
#include <QPainterPath>
#include <algorithm>
#include <cmath>
#include <memory>
#include <numeric>
#include <type_traits>

namespace {
constexpr double pi = 3.14159265358979323846;
double finiteValue(double value) { return std::isfinite(value) ? value : 0; }
double unit(double value) { return std::clamp(finiteValue(value), 0., 1.); }
// Every frozen frame must be released, including empty frames and exceptions.
template <typename Source> struct FrameRelease {
    Source *source;
    ~FrameRelease() { source->release(); }
};
} // namespace

ClamQuickMonitor::ClamQuickMonitor(QQuickItem *parent) : QQuickPaintedItem(parent) {
    setImplicitWidth(260);
    setImplicitHeight(140);
    setAntialiasing(true);
    _timer.setInterval(50);
    connect(&_timer, &QTimer::timeout, this, &ClamQuickMonitor::refresh);
    connect(this, &ClamQuickMonitor::appearanceChanged, this, [this] { update(); });
    _timer.start();
}

void ClamQuickMonitor::setMonitorType(Type type) {
    // Changing the data type of a live binding needs a new interface/network.
    if (_arraySource || _peakSource || _segmentSource || _type == type)
        return;
    _type = type;
    emit appearanceChanged();
}

void ClamQuickMonitor::setRefreshInterval(int ms) {
    _timer.setInterval(std::max(16, ms));
    emit appearanceChanged();
}

template <typename Monitor>
bool ClamQuickMonitor::bindMonitor(CLAM::Network &network, const std::string &port, QStringList &errors) {
    auto monitor = std::make_unique<Monitor>();
    const std::string name = network.GetUnusedName("QmlMonitor");
    Monitor *source = monitor.get();
    network.AddProcessing(name, monitor.release());
    if (!network.ConnectPorts(port, name + ".Input")) {
        network.RemoveProcessing(name);
        return CLAM::QtBinder::error(
            errors,
            tr("Monitor type %1 is incompatible with '%2'.").arg(_type).arg(QString::fromStdString(port)));
    }
    if constexpr (std::is_base_of_v<CLAM::VM::FloatArrayDataSource, Monitor>)
        _arraySource = source;
    if constexpr (std::is_base_of_v<PeakDataSource, Monitor>)
        _peakSource = source;
    if constexpr (std::is_base_of_v<CLAM::VM::SegmentationDataSource, Monitor>)
        _segmentSource = source;
    return true;
}

bool ClamQuickMonitor::bind(CLAM::Network &network, QStringList &errors) {
    if (_arraySource || _peakSource || _segmentSource)
        return true;
    auto port = _outPort.toStdString();
    if (port.empty() && objectName().startsWith("OutPort__"))
        port = CLAM::QtBinder::widget2NetworkName("OutPort__", objectName());
    if (port.empty())
        return true; // Unbound components can be composed and previewed in QML tools.
    if (port.find('.') == std::string::npos)
        return CLAM::QtBinder::error(
            errors, tr("clamOutPort must be 'Processing.Port': %1").arg(QString::fromStdString(port)));
    if (CLAM::QtBinder::missingOutPort(port, network, errors))
        return false;
#define CLAM_BIND_MONITOR(kind)                                                                              \
    case kind:                                                                                               \
        return bindMonitor<kind##Monitor>(network, port, errors)
    switch (_type) {
        CLAM_BIND_MONITOR(Oscilloscope);
        CLAM_BIND_MONITOR(BufferOscilloscope);
        CLAM_BIND_MONITOR(Vumeter);
        CLAM_BIND_MONITOR(SpectrumView);
        CLAM_BIND_MONITOR(PeakView);
        CLAM_BIND_MONITOR(VectorView);
        CLAM_BIND_MONITOR(HistogramView);
        CLAM_BIND_MONITOR(LPModelView);
        CLAM_BIND_MONITOR(MelSpectrumView);
        CLAM_BIND_MONITOR(MelCepstrumView);
        CLAM_BIND_MONITOR(Spectrogram);
        CLAM_BIND_MONITOR(Tonnetz);
        CLAM_BIND_MONITOR(KeySpace);
        CLAM_BIND_MONITOR(ChordRanking);
        CLAM_BIND_MONITOR(PolarChromaPeaks);
        CLAM_BIND_MONITOR(Tunner);
        CLAM_BIND_MONITOR(SegmentationView);
    }
#undef CLAM_BIND_MONITOR
    return CLAM::QtBinder::error(errors, tr("Unknown QML monitor type."));
}

void ClamQuickMonitor::refresh() {
    // Snapshot CLAM on the GUI thread. paint() can run on Qt's render thread
    // and only reads these owned copies while Qt blocks the GUI thread.
    if (_arraySource && _arraySource->isEnabled()) {
        const auto *data = _arraySource->frameData();
        FrameRelease<CLAM::VM::FloatArrayDataSource> release{_arraySource};
        const unsigned size = _arraySource->nBins();
        _values.clear();
        _labels.clear();
        if (data && size)
            _values.assign(data, data + size);
        for (auto &value : _values)
            value = finiteValue(value);
        if (_type == ChordRanking || _type == Tonnetz || _type == KeySpace)
            for (unsigned i = 0; i < _values.size(); ++i)
                _labels << QString::fromStdString(_arraySource->getLabel(i));
        _sourceLower = _arraySource->hasLowerBound() ? _arraySource->lowerBound() : _lowerBound;
        _sourceUpper = _arraySource->hasUpperBound() ? _arraySource->upperBound() : _upperBound;
        if (_type == Spectrogram && !_values.empty()) {
            if (_history.size() != QSize(256, int(_values.size()))) {
                _history = QImage(256, int(_values.size()), QImage::Format_RGB32);
                _history.fill(_backgroundColor);
            }
            const double maximum = std::max(1e-12, double(*std::max_element(_values.begin(), _values.end())));
            for (int y = 0; y < _history.height(); ++y) {
                auto *row = reinterpret_cast<QRgb *>(_history.scanLine(y));
                std::move(row + 1, row + _history.width(), row);
                const double value = unit(_values[_values.size() - 1 - y] / maximum);
                row[_history.width() - 1] = QColor::fromHsvF(.67 * (1 - value), 1, value).rgb();
            }
        }
    } else if (_peakSource && _peakSource->isEnabled()) {
        const auto *positions = _peakSource->positionData();
        const auto *values = _peakSource->magnitudeData();
        const unsigned size = _peakSource->nBins();
        _values.clear();
        _positions.clear();
        if (positions && values && size) {
            _values.assign(values, values + size);
            _positions.assign(positions, positions + size);
            for (auto &value : _values)
                value = finiteValue(value);
            for (auto &value : _positions)
                value = finiteValue(value);
        }
    } else if (_segmentSource && _segmentSource->isEnabled()) {
        const auto &frame = _segmentSource->frameData();
        FrameRelease<CLAM::VM::SegmentationDataSource> release{_segmentSource};
        _currentTime = _segmentSource->currentTime();
        _segments.clear();
        for (unsigned i = 0; i < frame.onsets().size(); ++i)
            _segments.push_back(
                {frame.onsets()[i], frame.offsets()[i], QString::fromStdString(frame.labels()[i])});
    } else
        return;
    emit frameChanged();
    update();
}

void ClamQuickMonitor::paint(QPainter *painter) {
    painter->fillRect(boundingRect(), _backgroundColor);
    const QRectF plot = boundingRect().adjusted(8, 8, -8, -8);
    if (plot.width() <= 0 || plot.height() <= 0)
        return;
    painter->setClipRect(plot);
    painter->setPen(QPen(QColor("#405363"), 1));
    painter->drawRect(plot);
    painter->setPen(QPen(_lineColor, 1.5));
    if (_type == SegmentationView) {
        const double span = std::max(.01, finiteValue(_timeSpan));
        for (const auto &segment : _segments) {
            const double start = (segment.onset - _currentTime + span) / span;
            const double end = (segment.offset - _currentTime + span) / span;
            const QRectF box(plot.left() + start * plot.width(), plot.top(), (end - start) * plot.width(),
                             plot.height());
            painter->fillRect(box.adjusted(1, 1, -1, -1), _lineColor.darker(230));
            painter->drawRect(box);
            painter->drawText(box, Qt::AlignCenter | Qt::TextWordWrap, segment.label);
        }
        return;
    }
    if (_values.empty()) {
        painter->drawText(plot, Qt::AlignCenter,
                          _outPort.isEmpty() ? tr("QML preview") : tr("Waiting for audio"));
        return;
    }
    if (_type == Spectrogram) {
        painter->drawImage(plot, _history);
        return;
    }
    if (_type == Vumeter) {
        double energy = 0;
        for (double value : _values)
            energy += value * value;
        const double rms = std::sqrt(energy / _values.size());
        const double level = unit((20 * std::log10(std::max(rms, 1e-6)) + 60) / 60);
        for (int i = 0; i < 20; ++i) {
            const double y = plot.bottom() - (i + 1) * plot.height() / 20;
            painter->fillRect(QRectF(plot.left() + 2, y + 1, plot.width() - 4, plot.height() / 20 - 2),
                              i / 20. < level ? (i > 17   ? QColor("#ed5353")
                                                 : i > 13 ? QColor("#e8c94c")
                                                          : QColor("#65d598"))
                                              : QColor("#283746"));
        }
        return;
    }
    if (_type == PolarChromaPeaks || _type == Tunner) {
        const QPointF centre = _type == Tunner ? QPointF(plot.center().x(), plot.bottom()) : plot.center();
        const double radius =
            std::min(plot.width() / 2, _type == Tunner ? plot.height() : plot.height() / 2) * .85;
        painter->drawEllipse(centre, radius, radius);
        double maximum = std::max(1e-12, double(*std::max_element(_values.begin(), _values.end())));
        for (unsigned i = 0; i < _values.size(); ++i) {
            const double angle = _type == Tunner ? pi * (_positions[i] - .5) : -pi * _positions[i] / 6;
            const double length = radius * unit(_values[i] / maximum);
            const QPointF end = centre + QPointF(std::cos(angle), std::sin(angle)) * length;
            painter->drawLine(centre, end);
            painter->drawEllipse(end, 3, 3);
        }
        return;
    }
    const double maximum = std::max(1e-12, double(*std::max_element(_values.begin(), _values.end())));
    if (_type == Tonnetz || _type == KeySpace) {
        const int columns = _type == KeySpace ? 12 : 6;
        const int rows = _type == KeySpace ? 2 : 3;
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < columns; ++col) {
                // Tonnetz neighbours follow perfect fifths horizontally and major thirds vertically.
                const unsigned index = _type == KeySpace ? row * 12 + col : (col * 7 + row * 4) % 12;
                if (index >= _values.size())
                    continue;
                const QRectF cell(plot.left() + col * plot.width() / columns,
                                  plot.top() + row * plot.height() / rows, plot.width() / columns,
                                  plot.height() / rows);
                painter->fillRect(cell.adjusted(2, 2, -2, -2),
                                  QColor::fromHsvF(.55, .8, .2 + .8 * unit(_values[index] / maximum)));
                painter->drawText(cell, Qt::AlignCenter, _labels.value(index));
            }
        return;
    }
    if (_type == ChordRanking) {
        std::vector<unsigned> order(_values.size());
        std::iota(order.begin(), order.end(), 0);
        std::stable_sort(order.begin(), order.end(),
                         [this](unsigned a, unsigned b) { return _values[a] > _values[b]; });
        const unsigned count = std::min<unsigned>(8, order.size());
        for (unsigned i = 0; i < count; ++i) {
            const QRectF row(plot.left(), plot.top() + i * plot.height() / count, plot.width(),
                             plot.height() / count);
            painter->fillRect(QRectF(row.topLeft(), QSizeF(row.width() * unit(_values[order[i]] / maximum),
                                                           row.height() - 2)),
                              _lineColor.darker(250));
            painter->drawText(row.adjusted(5, 0, -5, 0), Qt::AlignVCenter, _labels.value(order[i]));
        }
        return;
    }
    if (_type == PeakView) {
        const double range = std::max(1., double(*std::max_element(_positions.begin(), _positions.end())));
        for (unsigned i = 0; i < _values.size(); ++i) {
            const double x = plot.left() + unit(_positions[i] / range) * plot.width();
            painter->drawLine(QPointF(x, plot.bottom()),
                              QPointF(x, plot.bottom() - unit((_values[i] + 100) / 100) * plot.height()));
        }
        return;
    }
    double low = _sourceLower, high = _sourceUpper;
    if (_type == VectorView || _type == HistogramView) {
        low = std::min(0., double(*std::min_element(_values.begin(), _values.end())));
        high = std::max(low + 1e-12, maximum);
    }
    const double extent = std::max(1e-12, high - low);
    QPolygonF line;
    for (unsigned i = 0; i < _values.size(); ++i) {
        double value = (_values[i] - low) / extent;
        if (_type == SpectrumView || _type == MelSpectrumView)
            value = (20 * std::log10(std::max(1e-12, double(_values[i]) / _values.size())) + 140) / 140;
        const double x = plot.left() + i * plot.width() / std::max<size_t>(1, _values.size() - 1);
        const double y = plot.bottom() - unit(value) * plot.height();
        if (_type == HistogramView)
            painter->drawLine(QPointF(x, plot.bottom()), QPointF(x, y));
        else
            line << QPointF(x, y);
    }
    if (_type != HistogramView)
        painter->drawPolyline(line);
}
