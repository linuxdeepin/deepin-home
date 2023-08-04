// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import "../widgets"

Control {
    id: root
    property var selectedNotify
    property int index
    signal listHide()
    
    InWindowBlur {
        id: blur
        anchors.fill: sidebar
        radius: 20
        offscreen: false
    }
    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(0,0,0,0.2)
        
        MouseArea {
            width: parent.width*0.6
            height: parent.height
            onClicked: {
                destroyAnim.start()
            }
        }
        MouseArea {
            x: parent.width*0.6
            width: parent.width*0.4
            height: parent.height
        }
    }
    // 列表隐藏动画
    SequentialAnimation {
        id: destroyAnim
        NumberAnimation { 
            target: sidebar
            property: "x"
            to: parent.width
        }
        ScriptAction { 
            script: {
                root.listHide()
            }
        }
    }
    RoundRectangle {
        id: sidebar
        x: root.width
        width: root.width*0.4
        height: root.height
        color: Qt.rgba(1,1,1,0.8)
        radius: 18
        clip: true
        corners: (RoundRectangle.LeftCorner)
        // 列表出现动画
        Behavior on x { PropertyAnimation {} }
        Component.onCompleted: {
            x = root.width * 0.6
        }
        Component {
            id: list
            Item {
                TabBar {
                    id: bar
                    x: 10
                    y: 10
                    width: sidebar.width-10*2
                    currentIndex: root.index
                    TabButton {
                        text: qsTr("Messages")
                    }
                    TabButton {
                        text: qsTr("Questionaires")
                    }
                    onCurrentIndexChanged: {
                        root.index = currentIndex
                    }
                }
                Item {
                    anchors.top: bar.bottom
                    anchors.topMargin: 10
                    anchors.bottom: parent.bottom
                    width: parent.width - 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    Loader {
                        id: list_loader
                        anchors.fill: parent
                        source: ["Notify.qml", "Questionnaire.qml"][root.index]
                        Connections {
                            target: list_loader.item
                            ignoreUnknownSignals: true
                            function onSelected(notify) {
                                root.selectedNotify = notify
                            }
                        }
                    }
                }
            }
        }
        Component {
            id: detail
            NotifyDetail {
                title: root.selectedNotify.title
                content: root.selectedNotify.content
                start_at: root.selectedNotify.start_at
                url: root.selectedNotify.url
                onCanceled: {
                    root.selectedNotify = null
                }
            }
        }
        Loader {
            anchors.fill: parent
            sourceComponent: root.selectedNotify == null ? list : detail
        }
    }
}