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
    Rectangle {
        width: root.width
        height: root.height
        radius: 20
        LinearGradient {
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
            font.pointSize: 16
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            anchors.top: title.bottom
            anchors.topMargin: 11
            anchors.horizontalCenter: parent.horizontalCenter

            width: parent.width-39
            fillMode: Image.PreserveAspectFit
            source: image
        }
    }
}