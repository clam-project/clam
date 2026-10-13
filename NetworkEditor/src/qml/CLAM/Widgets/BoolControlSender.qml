// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls

CheckBox {
    id: root
    property alias clamInControl: binding.clamInControl
    property alias clamProcessing: binding.clamProcessing
    ControlBinding {
        id: binding
        value: root.checked
    }
}
