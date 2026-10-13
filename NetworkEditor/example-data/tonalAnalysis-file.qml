// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CLAM.Widgets 1.0

ApplicationWindow {
    width: 1000
    height: 680
    title: qsTr("CLAM Tonal Analysis")
    visible: typeof prototyper === "undefined"
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        RowLayout {
            Label { text: qsTr("Tonal analysis"); font.pixelSize: 24; Layout.fillWidth: true }
            Button { text: qsTr("Open audio…"); onClicked: reader.chooseAudioFile() }
            Button { text: qsTr("Analysis settings…"); onClicked: analysis.configure() }
            ControlBinding { id: reader; clamProcessing: "MonoAudioFileReader_0" }
            ControlBinding { id: analysis; clamProcessing: "TonalAnalysis_0" }
        }
        GridLayout {
            columns: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Tonnetz { clamOutPort: "TonalAnalysis_0.Pitch Profile"; Layout.fillWidth: true; Layout.fillHeight: true }
            ChordRanking { clamOutPort: "TonalAnalysis_0.Chord Correlation"; Layout.fillWidth: true; Layout.fillHeight: true }
            PolarChromaPeaks { clamOutPort: "TonalAnalysis_0.Chroma Peaks"; Layout.fillWidth: true; Layout.fillHeight: true }
            KeySpace { clamOutPort: "TonalAnalysis_0.Chord Correlation"; Layout.fillWidth: true; Layout.fillHeight: true }
            Oscilloscope { clamOutPort: "MonoAudioFileReader_0.Samples Read"; Layout.fillWidth: true; Layout.fillHeight: true }
            Spectrogram { clamOutPort: "TonalAnalysis_0.Pitch Profile"; Layout.fillWidth: true; Layout.fillHeight: true }
        }
        PlaybackControls { controller: typeof prototyper !== "undefined" ? prototyper : null; Layout.fillWidth: true }
    }
}
