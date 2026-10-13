// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls
import CLAM.Widgets

SpinBox {
    id: root
    property alias clamInControl: binding.clamInControl
    property alias clamProcessing: binding.clamProcessing
    property int decimals: 2
    property real controlValue: value / Math.pow(10, decimals)
    editable: true
    to: 10000
    textFromValue: function(value, locale) { return Number(value / Math.pow(10, decimals)).toLocaleString(locale, 'f', decimals) }
    valueFromText: function(text, locale) { return Math.round(Number.fromLocaleString(locale, text) * Math.pow(10, decimals)) }
    validator: DoubleValidator {
        bottom: Math.min(root.from, root.to) / Math.pow(10, root.decimals)
        top: Math.max(root.from, root.to) / Math.pow(10, root.decimals)
        decimals: root.decimals
    }
    ControlBinding { id: binding; value: root.controlValue }
}
