// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CLAM.Widgets 1.0

ApplicationWindow {
    width: 420
    height: 150
    title: qsTr("CLAM Prototyper")
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        Label { text: qsTr("Network playback"); font.pixelSize: 20 }
        PlaybackControls { controller: prototyper; Layout.fillWidth: true }
    }
}
