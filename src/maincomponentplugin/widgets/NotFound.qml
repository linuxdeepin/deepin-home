// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.deepin.dtk 1.0

// 无数据时显示
Item {
    property string title
    Image {
        sourceSize.width: 130
        sourceSize.height: 130
        source: "/images/404.svg"
        anchors.bottom: title_text.top
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Text {
        id: title_text
        text: title
        font.pixelSize: 14
        
        anchors.centerIn: parent
        opacity: 0.4
    }
}