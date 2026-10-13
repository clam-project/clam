// SPDX-License-Identifier: GPL-2.0-or-later
pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root
    property alias clamProcessing: binding.clamProcessing
    property int firstNote: 60
    property int velocity: 90
    implicitWidth: 280
    implicitHeight: 100
    ControlBinding { id: binding }
    Repeater {
        model: [0, 2, 4, 5, 7, 9, 11]
        delegate: Rectangle {
            id: whiteKey
            required property int index
            required property int modelData
            x: index * root.width / 7
            width: root.width / 7; height: root.height
            border.color: "#405363"
            color: whiteMouse.pressed ? "#52b7dc" : "#f0f2f5"
            MouseArea {
                id: whiteMouse
                anchors.fill: parent
                property int pressedNote: 0
                onPressed: { pressedNote = root.firstNote + whiteKey.modelData; binding.sendMidi(pressedNote, true, root.velocity) }
                onReleased: binding.sendMidi(pressedNote, false)
                onCanceled: binding.sendMidi(pressedNote, false)
            }
        }
    }
    Repeater {
        model: [1, 3, 6, 8, 10]
        delegate: Rectangle {
            id: blackKey
            required property int index
            required property int modelData
            x: ([1, 2, 4, 5, 6][index] - 0.3) * root.width / 7
            width: root.width / 7 * 0.6; height: root.height * 0.62
            color: blackMouse.pressed ? "#52b7dc" : "#15212c"
            MouseArea {
                id: blackMouse
                anchors.fill: parent
                property int pressedNote: 0
                onPressed: { pressedNote = root.firstNote + blackKey.modelData; binding.sendMidi(pressedNote, true, root.velocity) }
                onReleased: binding.sendMidi(pressedNote, false)
                onCanceled: binding.sendMidi(pressedNote, false)
            }
        }
    }
}
