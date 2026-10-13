// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick

Rectangle {
    id: root
    property alias clamXControl: xBinding.clamInControl
    property alias clamYControl: yBinding.clamInControl
    property alias clamProcessing: surfaceBinding.clamProcessing
    property real minimumX: 0
    property real maximumX: 1
    property real minimumY: 0
    property real maximumY: 1
    property real valueX: (minimumX + maximumX) / 2
    property real valueY: (minimumY + maximumY) / 2
    implicitWidth: 240
    implicitHeight: 160
    color: "#15212c"
    border.color: "#52b7dc"
    ControlBinding { id: xBinding; value: root.valueX }
    ControlBinding { id: yBinding; value: root.valueY }
    ControlBinding { id: surfaceBinding }
    Rectangle {
        width: 10; height: 10; radius: 5; color: "#52b7dc"
        x: (root.valueX - root.minimumX) / Math.max(0.00001, root.maximumX - root.minimumX) * root.width - 5
        y: (1 - (root.valueY - root.minimumY) / Math.max(0.00001, root.maximumY - root.minimumY)) * root.height - 5
    }
    MouseArea {
        anchors.fill: parent
        function movePoint(mx: real, my: real) {
            root.valueX = root.minimumX + Math.max(0, Math.min(1, mx / width)) * (root.maximumX - root.minimumX)
            root.valueY = root.minimumY + (1 - Math.max(0, Math.min(1, my / height))) * (root.maximumY - root.minimumY)
            surfaceBinding.sendSurface(root.valueX, root.valueY)
        }
        onPressed: mouse => movePoint(mouse.x, mouse.y)
        onPositionChanged: mouse => { if (pressed) movePoint(mouse.x, mouse.y) }
    }
}
