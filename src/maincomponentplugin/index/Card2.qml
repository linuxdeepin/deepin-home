// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later


import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import QtGraphicalEffects 1.0
import org.deepin.dtk 1.0

Control {
    id: root
    property string title
    property string image
    property Gradient gradient
    property color shadowColor
    property bool hover
    signal clicked()

    Rectangle {
        id: boxShadowSource
        width: root.width
        height: root.height
        y: root.hover ? -6 : 0
        Behavior on y { PropertyAnimation {} }
        radius: 20
        clip: true
        LinearGradient {
            id: g
            source: parent
            anchors.fill: parent
            start: Qt.point(0, 0)
            end: Qt.point(width, height)
            gradient: root.gradient
        }
        Text {
            id: title
            y: 16
            text: root.title
            font.pixelSize: 16
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
        // 阴影
        BoxShadow {
            anchors.fill: cover_image
            shadowBlur : 10
            shadowColor : root.shadowColor
            shadowOffsetX : 20
            shadowOffsetY : 2
            cornerRadius: 20
            opacity: 0.5
        }
        Image {
            id: cover_image
            anchors.top: title.bottom
            anchors.topMargin: 11
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: parent.width-40
            source: image
        }
    }
    // 阴影
    BoxShadow {
        anchors.fill: boxShadowSource
        shadowBlur : 12
        shadowColor : Qt.rgba(0.07, 0.2, 0.5, root.hover ? 0.1 : 0.05)
        Behavior on shadowColor { PropertyAnimation {} }
        shadowOffsetX : 0
        shadowOffsetY : 5
        cornerRadius: boxShadowSource.radius
        hollow: true
    }
    MouseArea {
        width: root.width
        height: root.height
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: {
            root.hover = true
        }
        onExited: {
            root.hover = false
        }
        onClicked: {
            root.clicked()
        }
    }
}