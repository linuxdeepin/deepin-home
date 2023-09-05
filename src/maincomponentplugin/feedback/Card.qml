// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later

import "../widgets"
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0

// 显示用户反馈的详细信息，在反馈列表和反馈详情中使用
// 在反馈列表中，截图缩小横排显示
Rectangle {
    id: root

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
    property bool inList: true // 卡片是否在列表中
    property bool isReply: false // 卡片是否为回复
    property int view_count: 0
    property int like_count: 0
    property int collect_count: 0
    property string system_version: ''

    signal titleClicked()
    signal likeClicked()
    signal collectClicked()
    signal linkClicked(string url)
    signal imageClicked(string url)

    radius: 18
    height: column.height

    // 用户头像
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

        x: 20 + 36 + 10
        width: parent.width - x - 20
        clip: true
        spacing: 0

        // 反馈标题
        Text {
            id: titleText

            Layout.maximumWidth: parent.width - 100
            Layout.topMargin: 10
            font.pixelSize: DTK.fontManager.t5.pixelSize
            font.weight: Font.Medium
            wrapMode: Text.WrapAnywhere
            elide: Text.ElideRight
            maximumLineCount: inList ? 1 : 0
            text: root.title
            color: titleTextArea.containsMouse ? "#1979ff" : ''

            MouseArea {
                id: titleTextArea

                visible: inList
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onClicked: {
                    root.titleClicked();
                }
            }

        }

        // 反馈时间
        Text {
            id: subtitleText

            Layout.topMargin: 2
            color: "gray"
            font: DTK.fontManager.t9
            text: new Date(root.created_at).toLocaleString(locale, Locale.ShortFormat) + "  " + (root.isReply ? '' : qsTr("%1 views").arg(root.view_count))
        }

        // 反馈内容
        Text {
            id: contentText

            visible: !isReply
            Layout.fillWidth: true
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            wrapMode: Text.WrapAnywhere
            elide: Text.ElideRight
            maximumLineCount: inList ? 2 : 0
            text: root.content
            color: Qt.rgba(0, 0, 0, 0.7)
        }

        // 官方回复
        TextEdit {
            id: reply

            visible: isReply
            Layout.fillWidth: true
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            readOnly: true
            wrapMode: TextEdit.WrapAnywhere
            textFormat: TextEdit.RichText
            selectByMouse: true
            text: root.content
            color: Qt.rgba(0, 0, 0, 0.7)

            MouseArea {
                id: replyMouseArea

                visible: reply.hoveredLink
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.linkClicked(reply.hoveredLink);
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
                            imageClicked(root.screenshots[index]);
                        }
                    }

                }

            }

        }

        // 在反馈详情中纵向展示全尺寸截图
        Repeater {
            model: root.inList ? [] : root.screenshots

            delegate: Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: root.screenshots[index].endsWith(".gif") ? screenshotImg.height : screenshotImgStatic.height

                AnimatedImage {
                    id: screenshotImg

                    visible: root.screenshots[index].endsWith(".gif")
                    width: parent.width
                    fillMode: Image.PreserveAspectFit
                    source: root.screenshots[index]

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onClicked: {
                            imageClicked(root.screenshots[index]);
                        }
                    }

                }

                Image {
                    id: screenshotImgStatic

                    visible: !root.screenshots[index].endsWith(".gif")
                    width: parent.width
                    fillMode: Image.PreserveAspectFit
                    source: root.screenshots[index]

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onClicked: {
                            imageClicked(root.screenshots[index]);
                        }
                    }

                }

            }

        }

        // 底部信息
        RowLayout {
            visible: !root.isReply
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            Layout.fillWidth: true

            // 反馈分类
            Rectangle {
                width: typeText.width + 15
                height: typeText.height + 4
                border.width: 1
                border.color: root.type === 'bug' ? "#eead86" : "#0d9353"
                radius: 5

                Text {
                    id: typeText

                    text: root.type === 'bug' ? "BUG" : qsTr("Suggestions")
                    color: root.type === 'bug' ? "#eead86" : "#0d9353"
                    font: DTK.fontManager.t9
                    anchors.centerIn: parent
                }

            }

            // 系统版本
            Rectangle {
                visible: root.system_version
                Layout.leftMargin: 5
                width: versionText.width + 15
                height: versionText.height + 4
                border.width: 1
                border.color: "#b6b6b6"
                radius: 5

                Text {
                    id: versionText

                    color: "#838383"
                    text: "V" + root.system_version
                    font: DTK.fontManager.t9
                    anchors.centerIn: parent
                }

            }

            Item {
                Layout.fillWidth: true
            }

            // 点赞和收藏
            RowLayout {
                spacing: 30
                visible: root.status != 'submit' // 待处理的反馈不会展示在广场，也不需要展示统计数据

                MouseArea {
                    width: childrenRect.width
                    height: childrenRect.height
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                    onClicked: {
                        root.collectClicked();
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
                            opacity: root.collect ? 0.7 : 1
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
                    width: childrenRect.width
                    height: childrenRect.height
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                    onClicked: {
                        root.likeClicked();
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
                            opacity: root.like ? 0.7 : 1
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

    // 反馈状态加载器，不在回复模式加载状态
    Loader {
        anchors.right: parent.right
        anchors.top: parent.top
        sourceComponent: !root.isReply ? statusComponent : null

        // 反馈状态
        Component {
            id: statusComponent

            Status {
                type: root.type
                status: root.status
            }

        }

    }

    // 阴影
    BoxShadow {
        anchors.fill: parent
        shadowBlur: 6
        shadowColor: Qt.rgba(0, 0, 0, 0.1)
        shadowOffsetX: 0
        shadowOffsetY: 1
        cornerRadius: root.radius
        hollow: true
    }

}
