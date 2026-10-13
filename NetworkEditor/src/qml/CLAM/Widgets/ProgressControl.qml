// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls
import CLAM.Widgets

Slider {
    id: root
    property string clamProcessing: ""
    property alias clamInControl: binding.clamInControl
    property alias clamOutControl: binding.clamOutControl
    from: 0
    to: 1
    onMoved: binding.send(value)
    ControlBinding {
        id: binding
        clamInControl: root.clamProcessing ? root.clamProcessing + ".Current Time Position (%)" : ""
        clamOutControl: root.clamProcessing ? root.clamProcessing + ".Current Time Position" : ""
        onObservedValueChanged: { if (!root.pressed) root.value = Number(observedValue) }
    }
}
