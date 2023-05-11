// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import org.deepin.dtk 1.0

import '../router'

// 离线断网界面
Rectangle {
    Image {
        id: err_image
        anchors.centerIn: parent
        source: "/images/404.svg"
    }
    Text {
        id: err_title
        text: qsTr("Network error, please try later")
        font.pixelSize: 15
        anchors.top: err_image.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Button {
        text: qsTr("Refresh")
        anchors.top: err_title.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            Router.showIndex()
        }
    }
}