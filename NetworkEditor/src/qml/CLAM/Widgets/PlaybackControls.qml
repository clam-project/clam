// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    property var controller: null
    Button { text: qsTr("Play"); enabled: root.controller ? !root.controller.playing : false; onClicked: root.controller.Start() }
    Button { text: qsTr("Pause"); enabled: root.controller ? root.controller.playing : false; onClicked: root.controller.Pause() }
    Button { text: qsTr("Stop"); enabled: root.controller ? !root.controller.stopped : false; onClicked: root.controller.Stop() }
    Label {
        text: root.controller ? root.controller.playbackStatus : qsTr("Preview")
        Layout.fillWidth: true
    }
    Label { text: root.controller ? root.controller.backendName : "" }
}
