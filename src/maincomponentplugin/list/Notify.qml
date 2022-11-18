// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0
import "../api"
import "../widgets"

Control {
    id: root
    signal selected(var notify)
    property bool loaded

    ListModel {
        id: list_model
    }

    Component.onCompleted: {
        API.getNotify((result) => {
            let locale = Qt.locale()
            for(let item of result){
                list_model.append({
                    "uuid": item.uuid,
                    "title": item.title,
                    "summary": item.summary,
                    "start_at": new Date(item.start_at).toLocaleString(locale, Locale.ShortFormat),
                    "content": item.content,
                    "url": item.url,
                    "read": false,
                })
            }
            if(list_model.count == 0){
                container.sourceComponent = not_found
            } else {
                container.sourceComponent = list_view
            }
        })
    }

    // 加载动画
    Component {
        id: loading
        Loading {}
    }
    // 空数据
    Component {
        id: not_found
        NotFound {
            title: qsTr("No messages yet.")
        }
    }
    // 列表
    Component {
        id: list_view
        Item {
            anchors.fill: parent
            ListView {
                y: 30
                height: parent.height - y
                width: parent.widget
                model: list_model
                delegate: Rectangle {
                    width: sidebar.width - 20
                    height: 66
                    radius: 8
                    color: index%2==0 ? Qt.rgba(0,0,0,0.05) : 'transparent';
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onEntered: {
                            parent.color=index%2==0 ? Qt.rgba(0,0,0,0.2) : Qt.rgba(0,0,0,0.1)
                        }
                        onExited: {
                            parent.color=index%2==0 ? Qt.rgba(0,0,0,0.05) : 'transparent'
                        }
                        onPressed: {
                            parent.color=Qt.rgba(0,0,0,0.3)
                        }
                        onReleased: {
                            parent.color=index%2==0 ? Qt.rgba(0,0,0,0.05) : 'transparent'
                        }
                        onClicked: {
                            console.log("select notfiy", index, uuid)
                            API.markRead("p","n",uuid)
                            root.selected({title, content, start_at, url})
                            API.messageCount()
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        x: 10
                        y: 12
                        width: parent.width - x*2
                        height: parent.height - y*2
                        Text {
                            id: starttime_text
                            text: start_at
                            anchors.right: parent.right
                            color: Qt.rgba(0,0,0,0.6);
                        }
                        Text {
                            id: title_text
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.verticalCenter: starttime_text.verticalCenter
                            anchors.right: starttime_text.left
                            text: title
                            font.pixelSize: 14
                            font.bold: true
                            elide: Text.ElideRight
                        }
                        Rectangle {
                            id: mark_dot
                            width: 8
                            height: 8
                            radius: 4
                            color: read ? "#b1b1b1" : "red"
                            anchors.right: title_text.left
                            anchors.rightMargin: 6
                            anchors.verticalCenter: title_text.verticalCenter
                        }
                        Text {
                            x: 20
                            text: summary
                            anchors.top: title_text.bottom
                            color: Qt.rgba(0,0,0,0.7);
                            anchors.left: title_text.left
                            anchors.right: parent.right
                            elide: Text.ElideRight
                        }
                    }
                    Component.onCompleted: {
                        API.isRead("p", "n", uuid, (r) => {
                            read = r
                            console.log("read",r)
                        })
                    }
                }
            }

            Item {
                x: 10
                width: parent.width - 20
                height: 30
                Text {
                    text: qsTr("%1 messages").arg(list_model.count)
                }
                Text {
                    id: markread_button
                    text: qsTr("Mark all as read")
                    property bool hover;
                    color: hover ? '#0081FF' : '$000';
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    Image {
                        id: read_image
                        source: "qrc:///images/read.svg"
                        visible: false
                    }
                    ColorOverlay {
                        id: markread_icon
                        width: 12
                        height: 12
                        anchors.right: markread_button.left
                        anchors.rightMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        source: read_image
                        color: parent.hover ? "#0081FF" : "black";
                        transform: rotation
                        antialiasing: true
                    }
                    MouseArea {
                        anchors.left: markread_icon.left
                        anchors.right: markread_button.right
                        height: markread_button.height
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onEntered: {
                            parent.hover = true
                        }
                        onExited: {
                            parent.hover = false
                        }
                        onClicked: {
                            for(let i=0; i < list_model.count; i++){
                                let item = list_model.get(i)
                                API.markRead("p", "n", item.uuid)
                                item.read = true
                            }
                            API.messageCount()
                        }
                    }
                }
            }
        }
    }
    Loader {
        id: container
        anchors.fill: parent
        sourceComponent: loading
    }
}