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
    property string icon
    Rectangle {
        width: root.width
        height: root.height
        color: "transparent"
        Image {
            id: icon
            x: 10
            y: 10
            height: parent.height - y * 2
            fillMode: Image.PreserveAspectFit
            source: root.icon
        }
        Text {
            text: root.title
            font.pointSize: 20
            anchors.left: icon.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 14
        }

        DciIcon {
            name: "menu_arrow"
            width: 12
            height: 12
            anchors.right: parent.right
            anchors.rightMargin: 14
            anchors.verticalCenter: parent.verticalCenter
            theme: DTK.themeType
        }
    }
}