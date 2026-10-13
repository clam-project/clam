// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import CLAM.Widgets

Item {
    id: root
    property alias clamXControl: xBinding.clamInControl
    property alias clamYControl: yBinding.clamInControl
    property alias clamOrientationControl: rotationBinding.clamInControl
    property real viewX: 0
    property real viewY: -1
    property real orientation: 0
    property real elevation: 0
    property real sourceX: 0
    property real sourceY: 0
    signal posChanged(point position)
    signal xPosChanged(real x)
    signal yPosChanged(real y)
    implicitWidth: 320
    implicitHeight: 240
    focus: true
    onViewXChanged: { xPosChanged(viewX); posChanged(Qt.point(viewX, viewY)); scene.requestPaint() }
    onViewYChanged: { yPosChanged(viewY); posChanged(Qt.point(viewX, viewY)); scene.requestPaint() }
    // The orientation property already supplies orientationChanged().
    onOrientationChanged: scene.requestPaint()
    onElevationChanged: scene.requestPaint()
    onSourceXChanged: scene.requestPaint()
    onSourceYChanged: scene.requestPaint()
    ControlBinding { id: xBinding; value: root.viewX }
    ControlBinding { id: yBinding; value: root.viewY }
    ControlBinding { id: rotationBinding; value: root.orientation }
    MouseArea { anchors.fill: parent; onPressed: root.forceActiveFocus() }
    Keys.onPressed: event => {
        const step = event.modifiers & Qt.ShiftModifier ? 0.4 : 0.1
        const sine = Math.sin(root.orientation * Math.PI / 180)
        const cosine = Math.cos(root.orientation * Math.PI / 180)
        if (event.key === Qt.Key_Up) { root.viewX += sine * step; root.viewY += cosine * step }
        else if (event.key === Qt.Key_Down) { root.viewX -= sine * step; root.viewY -= cosine * step }
        else if (event.key === Qt.Key_Left || event.key === Qt.Key_Right) {
            const sign = event.key === Qt.Key_Left ? 1 : -1
            if (event.modifiers & Qt.AltModifier) { root.viewX += cosine * step * sign; root.viewY -= sine * step * sign }
            else root.orientation = (root.orientation + sign * 10 + 360) % 360
        } else if (event.key === Qt.Key_PageUp) root.elevation = Math.min(80, root.elevation + 10)
        else if (event.key === Qt.Key_PageDown) root.elevation = Math.max(-80, root.elevation - 10)
        else if (event.key === Qt.Key_A) root.sourceX += step
        else if (event.key === Qt.Key_D) root.sourceX -= step
        else if (event.key === Qt.Key_W) root.sourceY += step
        else if (event.key === Qt.Key_S) root.sourceY -= step
        else { event.accepted = false; return }
        event.accepted = true
    }
    Canvas {
        id: scene
        anchors.fill: parent
        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
        onPaint: {
            const ctx = getContext("2d")
            ctx.fillStyle = "#15212c"
            ctx.fillRect(0, 0, width, height)
            const sine = Math.sin(root.orientation * Math.PI / 180)
            const cosine = Math.cos(root.orientation * Math.PI / 180)
            const horizon = height * (0.45 + root.elevation / 180)
            function project(x, y, z) {
                const dx = x - root.viewX, dy = y - root.viewY
                const depth = dy * cosine + dx * sine
                if (depth < 0.05) return null
                return { x: width / 2 + (dx * cosine - dy * sine) * width / (2 * depth),
                         y: horizon + (0.5 - z) * height / (2 * depth), depth: depth }
            }
            ctx.strokeStyle = "#405363"
            for (let i = -10; i <= 10; ++i) {
                for (let axis = 0; axis < 2; ++axis) {
                    let previous = null
                    for (let j = -100; j <= 100; ++j) {
                        const point = project(axis ? j / 10 : i, axis ? i : j / 10, 0)
                        if (point && previous) {
                            ctx.beginPath(); ctx.moveTo(previous.x, previous.y); ctx.lineTo(point.x, point.y); ctx.stroke()
                        }
                        previous = point
                    }
                }
            }
            const source = project(root.sourceX, root.sourceY, 0.3)
            if (source) {
                ctx.fillStyle = "#52b7dc"
                ctx.beginPath()
                ctx.arc(source.x, source.y, Math.min(width / 2, 0.08 * width / source.depth), 0, 2 * Math.PI)
                ctx.fill()
            }
            ctx.fillStyle = "#e2e8ef"
            ctx.font = "12px sans-serif"
            ctx.fillText("Listener: " + root.viewX.toFixed(2) + ", " + root.viewY.toFixed(2) + "   " + root.orientation.toFixed(0) + "°", 10, 20)
            ctx.fillText("Emitter: " + root.sourceX.toFixed(2) + ", " + root.sourceY.toFixed(2), 10, 38)
        }
    }
}
