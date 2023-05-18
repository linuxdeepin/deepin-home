// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import org.deepin.dtk 1.0
import "../api"

// 通知图标
WindowButton {
    id: notifyButton
    Image {
        id: notify_icon
        source: "/images/msg.svg"
        sourceSize.width: 18
        sourceSize.height: 14
        visible: false
    }
    ColorOverlay {
        id: button_icon
        source: notify_icon
        width: 18
        height: 14
        anchors.centerIn: parent
        color: notifyButton.pressed ? "#0081FF" : "black";
        transform: rotation
        antialiasing: true
    }
    Rectangle {
        color: "red"
        visible: API.msgCount > 0
        anchors.left: button_icon.right
        anchors.leftMargin: -6
        anchors.bottom: button_icon.top
        anchors.bottomMargin: -6
        width: count_text.width + 6
        height: count_text.height
        radius: 5
        Text {
            id: count_text
            text: API.msgCount
            font: DTK.fontManager.t8
            color: "#fff"
            anchors.centerIn: parent
        }
    }
}