// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CLAM.Widgets 1.0

ApplicationWindow {
    width: 600
    height: 380
    title: qsTr("CLAM File Player")
    // Prototyper shows the window after binding it. Preview tools can show it too.
    visible: typeof prototyper === "undefined"
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        Label { text: qsTr("Audio file player"); font.pixelSize: 22 }
        RowLayout {
            Button { text: qsTr("Open audio…"); onClicked: reader.chooseAudioFile() }
            Button { text: qsTr("Configure…"); onClicked: reader.configure() }
            ControlBinding { id: reader; clamProcessing: "AudioFileMemoryLoader" }
        }
        RowLayout {
            Label { text: qsTr("Volume") }
            ControlSlider {
                from: 0; to: 1; value: 1
                clamInControl: "AudioMixer.Gain 0"
                Layout.fillWidth: true
            }
        }
        Oscilloscope { clamOutPort: "AudioMixer.Output Audio"; Layout.fillWidth: true; Layout.fillHeight: true }
        ProgressControl { clamProcessing: "AudioFileMemoryLoader"; Layout.fillWidth: true }
        PlaybackControls { controller: typeof prototyper !== "undefined" ? prototyper : null; Layout.fillWidth: true }
    }
}
