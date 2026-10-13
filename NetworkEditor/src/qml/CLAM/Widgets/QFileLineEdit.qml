// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls
import CLAM.Widgets

TextField {
    id: root
    property alias clamInControl: binding.clamInControl
    placeholderText: qsTr("File name")
    onEditingFinished: binding.send(text)
    ControlBinding { id: binding }
}
