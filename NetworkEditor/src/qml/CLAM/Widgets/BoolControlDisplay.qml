// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick

Rectangle {
    property alias clamOutControl: binding.clamOutControl
    property color activeColor: "#65d598"
    property color inactiveColor: "#405363"
    implicitWidth: 24
    implicitHeight: 24
    radius: Math.min(width, height) / 2
    color: binding.observedValue ? activeColor : inactiveColor
    ControlBinding { id: binding }
}
