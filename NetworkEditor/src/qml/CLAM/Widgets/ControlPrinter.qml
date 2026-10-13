// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls

Label {
    id: root
    property alias clamOutControl: binding.clamOutControl
    property int decimals: 3
    text: binding.observedValue === undefined ? "—" : Number(binding.observedValue).toFixed(decimals)
    ControlBinding { id: binding }
}
