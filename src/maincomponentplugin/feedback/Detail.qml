// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import "../api"
import "../widgets"

// 显示反馈的详情，在反馈列表中点击标题进入

Item {
    id: root
    property var feedback
    Rectangle {
        x: 20
        y: 10
        width: parent.width - x*2
        height: card.height
        Card {
            id: card
            width: parent.width
            inList: false
            public_id: feedback.public_id
            title: feedback.title
            content: feedback.content
            type: feedback.type
            status: feedback.status
            created_at: feedback.created_at
            like: feedback.like
            collect: feedback.collect
        }
    }
}