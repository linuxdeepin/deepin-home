// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import "../api"

// 显示用户反馈的详细信息，在反馈列表和反馈详情中使用
// 在反馈列表中，截图缩小横排显示，并且不显示回复的信息
Rectangle { 
    id: root
    radius: 10
    height: column.height
    property string public_id: ''
    property string title: ''
    property string content: ''
    property string created_at: ''
    property string type: ''
    property string status: ''
    property bool collect: false
    property bool like: false
    property bool inList: true // 是否在列表中
    signal titleClicked()

    // 反馈的当前状态
    RoundRectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        width: 88
        height: 36
        color: "#fff7e9"
        radius: 18
        corners: (RoundRectangle.BottomLeftCorner)
        RowLayout {
            anchors.centerIn: parent
            spacing: 5
            Text {
                text: "表"
            }
            Text {
                font: DTK.fontManager.t7
                text: root.status
            }
        }
    }
    // 头像预留位置
    Rectangle {
        x: 20
        y: 13
        width: 36
        height: 36
        radius: 36
        color: "gray"
    }
    // 详细信息在头像右边显示
    ColumnLayout {
        id: column
        x: 20+36+10
        width: parent.width - x - 20
        // 标题
        Text {
            id: titleText
            Layout.topMargin: 10
            font: DTK.fontManager.t4
            text: root.title

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.titleClicked()
                }
            }
        }
        // 时间
        Text {
            id: subtitleText
            Layout.topMargin: 2
            color: "gray"
            font: DTK.fontManager.t8
            text: new Date(root.created_at).toLocaleString(locale, Locale.ShortFormat) + " 1080次预览"
        }
        // 内容
        Text {
            id: contentText
            Layout.topMargin: 10
            Layout.fillWidth: true
            wrapMode: Text.Wrap
            text: root.content
        }
        // 在反馈列表中横向展示缩小的截图
        Row {
            visible: root.inList
            Layout.topMargin: 10
            spacing: 10
            Rectangle {
                width: 48
                height: 48
                color: "gray"
            }
            Rectangle {
                width: 48
                height: 48
                color: "gray"
            }
            Rectangle {
                width: 48
                height: 48
                color: "gray"
            }
        }
        // 在反馈详情中纵向展示全尺寸截图
        Repeater {
            model: root.inList ? [] : [1,2,3,4]
            delegate: Rectangle {
                width: 48
                height: 48
                color: "red"
            }
        }
        // 底部信息
        RowLayout {
            Layout.topMargin: 20
            Layout.bottomMargin: 10
            Layout.fillWidth: true
            // 反馈分类
            Rectangle {
                width: typeText.width+15
                height: typeText.height
                border.width: 1
                border.color: "#eead86"
                radius: 5
                Text {
                    id: typeText
                    text: root.type === 'bug' ? "BUG" : qsTr("需求")
                    color: root.type === 'bug' ? "#eead86" : '#0d9353'
                    font: DTK.fontManager.t6
                    anchors.centerIn: parent
                }
            }
            Item {
                Layout.fillWidth: true
            }
            // 点赞和收藏的数量
            RowLayout {
                spacing: 30
                MouseArea {
                    width:  childrenRect.width
                    height: childrenRect.height
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        // 点击可以收藏和取消收藏
                        const callback = () => {
                            root.collect = !root.collect
                        }
                        if (root.collect) {
                            API.cancelCollectFeedback(root.public_id, callback)
                        } else {
                            API.collectFeedback(root.public_id, callback)
                        }
                    }
                    RowLayout {
                        Image {
                            width: 22
                            height: 22
                            source: root.collect ? "/images/collect.svg" : "/images/collect-1.svg"
                        }
                        Text {
                            text: "1213"
                        }
                    }
                }
                MouseArea {
                    width:  childrenRect.width
                    height: childrenRect.height
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        // 点击可以点赞和取消点赞
                        const callback = () => {
                            root.like = !root.like
                        }
                        if (root.like) {
                            API.cancelLikeFeedback(root.public_id, callback)
                        } else {
                            API.likeFeedback(root.public_id, callback)
                        }
                    }
                    RowLayout {
                        Layout.leftMargin: 30
                        Image {
                            width: 22
                            height: 22
                            source: root.like ? "/images/hands-up.svg" : "/images/hands-up-1.svg"
                        }
                        Text {
                            text: "111"
                        }
                    }
                }
            }
        }
    }
    // 阴影
    BoxShadow {
        anchors.fill: parent
        shadowBlur : 18
        shadowColor : Qt.rgba(0,0,0,0.10)
        shadowOffsetX : 0
        shadowOffsetY : 1
        cornerRadius: root.radius
        hollow: true
    }
}