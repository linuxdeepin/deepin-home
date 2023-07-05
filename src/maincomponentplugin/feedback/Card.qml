// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import "../api"
import "../router"
import "../widgets" 

// 显示用户反馈的详细信息，在反馈列表和反馈详情中使用
// 在反馈列表中，截图缩小横排显示，并且不显示回复的信息
Rectangle { 
    id: root
    radius: 18
    height: column.height
    property string public_id: ''
    property string title: ''
    property string content: ''
    property string created_at: ''
    property string type: ''
    property string status: ''
    property bool collect: false
    property bool like: false
    property var screenshots: []
    property string nickname: ''
    property string avatar: ''
    property bool inList: true // 是否在列表中
    property bool isRelay: false // 是否为反馈
    property int view_count: 0
    property int like_count: 0
    property int collect_count: 0
    property string system_version: ''

    signal titleClicked()
    signal likeClicked()
    signal collectClicked()
    signal imageClicked(string url)

    // 头像预留位置
    Avatar {
        x: 20
        y: 13
        width: 36
        height: 36
        source: root.avatar
    }
    // 详细信息在头像右边显示
    ColumnLayout {
        id: column
        x: 20+36+10
        width: parent.width - x - 20
        clip: true
        spacing: 0
        // 标题
        Text {
            id: titleText
            Layout.maximumWidth: parent.width - 100
            Layout.topMargin: 10
            font: DTK.fontManager.t5
            wrapMode: Text.WrapAnywhere
            elide: Text.ElideRight
            maximumLineCount: inList ? 1 : 0
            text: root.title
            color: titleText.hovered ? 'red' : ''
            MouseArea {
                visible: inList
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onClicked: {
                    root.titleClicked()
                }
                ToolTip {
                    delay: 300
                    visible: parent.containsMouse
                    text: qsTr("View details")
                }
            }
        }
        // 时间
        Text {
            id: subtitleText
            Layout.topMargin: 2
            color: "gray"
            font: DTK.fontManager.t9
            text: new Date(root.created_at).toLocaleString(locale, Locale.ShortFormat) + "  " + (root.isRelay ? '' : qsTr("%1 views").arg(root.view_count))
        }
        // 内容
        Text {
            id: contentText
            Layout.fillWidth: true
            Layout.topMargin: 10
            Layout.bottomMargin: 5
            wrapMode: Text.WrapAnywhere
            elide: Text.ElideRight
            maximumLineCount: inList ? 2 : 0
            text: root.content
            color: Qt.rgba(0,0,0,0.7)
            // TODO 不知什么原因，无法触发root信号
            onLinkActivated: (link)=> {
                if(link.startsWith("#")){
                    const public_id = link.slice(1)
                    API.getFeedback({type: '', offset: 0, limit: 1, ids: [public_id]}, (resp)=>{
                        if(resp && resp[0]) {
                            const feedback = resp[0]
                            API.publicViewFeedback(feedback.public_id)
                            API.userViewFeedback(feedback.public_id)
                            Router.showFeedbackDetail(feedback)
                        }
                    })
                }
            }
        }
        // 在反馈列表中横向展示缩小的截图
        Row {
            visible: root.inList
            spacing: 10
            Repeater {
                model: root.screenshots
                delegate: Image {
                    source: root.screenshots[index]
                    width: 48
                    height: 48
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onClicked: {
                            imageClicked(root.screenshots[index])
                        }
                    }
                }
            }
        }
        // 在反馈详情中纵向展示全尺寸截图
        Repeater {
            model: inList ? [] : root.screenshots
            delegate: Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: screenshotImg.height
                AnimatedImage {
                    id: screenshotImg
                    visible: !root.inList && root.screenshots[index].endsWith(".gif")
                    width: parent.width
                    fillMode: Image.PreserveAspectFit
                    source: root.screenshots[index]
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onClicked: {
                            imageClicked(root.screenshots[index])
                        }
                    }
                }
                Image {
                    id: screenshotImgStatic
                    visible: !root.inList && !root.screenshots[index].endsWith(".gif")
                    width: parent.width
                    fillMode: Image.PreserveAspectFit
                    source: root.screenshots[index]
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onClicked: {
                            imageClicked(root.screenshots[index])
                        }
                    }
                }
            }
        }
        // 底部信息
        RowLayout {
            visible: !root.isRelay
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            Layout.fillWidth: true
            // 反馈分类
            Rectangle {
                width: typeText.width+15
                height: typeText.height+4
                border.width: 1
                border.color: root.type === 'bug' ? "#eead86" : "#0d9353"
                radius: 5
                Text {
                    id: typeText
                    text: root.type === 'bug' ? "BUG" : qsTr("Suggestions")
                    color: root.type === 'bug' ? "#eead86" : "#0d9353"
                    font: DTK.fontManager.t6
                    anchors.centerIn: parent
                }
            }

            Rectangle {
                visible: root.system_version
                Layout.leftMargin: 5
                width: versionText.width+15
                height: versionText.height+4
                border.width: 1
                border.color: "#b6b6b6"
                radius: 5
                Text {
                    id: versionText
                    color: "#838383"
                    text: "V"+root.system_version
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
                visible: root.status != 'submit' // 待处理的反馈不会展示在广场，也不需要展示统计数据
                MouseArea {
                    width:  childrenRect.width
                    height: childrenRect.height
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                    onClicked: {
                        root.collectClicked()
                    }
                    ToolTip {
                        delay: 300
                        visible: parent.containsMouse
                        text: root.collect ? qsTr("Unfavorite") : qsTr("Favorite")
                    }
                    RowLayout {
                        Image {
                            width: 22
                            height: 22
                            source: root.collect ? "/images/collect.svg" : "/images/collect-1.svg"
                        }
                        Text {
                            color: "gray"
                            font: DTK.fontManager.t9
                            text: root.collect_count
                        }
                    }
                }
                MouseArea {
                    width:  childrenRect.width
                    height: childrenRect.height
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                    onClicked: {
                        root.likeClicked()
                    }
                    ToolTip {
                        delay: 300
                        visible: parent.containsMouse
                        text: root.like ? qsTr("Cancel the urge") : qsTr("Urge")
                    }
                    RowLayout {
                        Layout.leftMargin: 30
                        Image {
                            width: 22
                            height: 22
                            source: root.like ? "/images/hands-up.svg" : "/images/hands-up-1.svg"
                        }
                        Text {
                            color: "gray"
                            font: DTK.fontManager.t9
                            text: root.like_count
                        }
                    }
                }
            }
        }
    }
    Status {
        visible: !root.isRelay
        anchors.right: parent.right
        anchors.top: parent.top
        type: root.type
        status: root.status
    }
    // 阴影
    BoxShadow {
        anchors.fill: parent
        shadowBlur : 10
        shadowColor : Qt.rgba(0,0,0,0.10)
        shadowOffsetX : 0
        shadowOffsetY : 1
        cornerRadius: root.radius
        hollow: true
    }
}