// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0

Rectangle {
    id: root
    property string title
    property string start_at
    property string content
    property string url
    color: "transparent"
    signal canceled()
    Button {
        id: btn
        x: 10
        y: 10
        width: 36
        height: 36
        icon.name: "arrow_ordinary_left"
        icon.width: 14
        icon.height: 14
        onClicked: {
            console.log("click")
            root.canceled()
        }
    }
    Rectangle {
        color: "transparent"
        anchors.top: btn.bottom
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        x: 20
        width: root.width - 40
        Text {
            width: parent.width
            id: title
            font.pixelSize: 20
            font.bold: true
            text: root.title
            wrapMode: Text.WordWrap
        }
        Text {
            anchors.top: title.bottom
            anchors.topMargin: 6
            id: time
            font.pixelSize: 11
            color: Qt.rgba(0,0,0,0.6)
            text: root.start_at
        }
        Text {
            width: parent.width
            anchors.top: time.bottom
            anchors.topMargin: 20
            font.pixelSize: 14
            text: root.content
            wrapMode: Text.WordWrap
            color: Qt.rgba(0,0,0,0.6)
        }
        RecommandButton {
            text: qsTr("View more")
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            visible: root.url
            onClicked: () => {
                Qt.openUrlExternally(root.url)
            }
        }
    }
}