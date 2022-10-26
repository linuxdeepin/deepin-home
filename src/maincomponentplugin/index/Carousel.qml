// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later


import QtQuick 2.0
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0


Control {
    id: root
    clip: true
    property ListModel model
    Rectangle {
        id: main
        anchors.fill: parent
        visible: false
        Component {
            id: delegate
            Item {
                id: item
                z: PathView.isCurrentItem ? 1 : 0
                Image {
                    id: raw
                    width: root.width
                    height: root.height
                    source: img
                }
            }
        }
        PathView {
            id: pathView
            model: root.model
            delegate: delegate 
            path: Path {
                startX: 0; startY: 0
                PathLine { x: root.width; y: pathView.height/2 }
                PathLine { x: root.width*2; y: pathView.height/2 }
            }
        }
    }
    Rectangle {
        id: mask
        width: root.width
        height: root.height
        radius: 10
        visible: false
    }
    OpacityMask {
        width: root.width
        height: root.height
        source: main
        maskSource: mask
    }
    Timer {
        id: timer
        interval: 3000
        repeat: true
        running: true
        onTriggered: {
            pathView.incrementCurrentIndex()
        }
    }
    Rectangle {
        width: 30
        height: 30
        radius: 15
        color: Qt.rgba(0, 0, 0, 0.1)
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        DciIcon {
            name: "arrow_ordinary_left"
            anchors.centerIn: parent
            sourceSize.width: 15
            sourceSize.height: 15
            theme: DTK.themeType
            palette {
                foreground: "#fff"
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                timer.restart()
                pathView.decrementCurrentIndex()
            }
        }
    }
    Rectangle {
        width: 30
        height: 30
        radius: 15
        color: Qt.rgba(0, 0, 0, 0.1)
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        DciIcon {
            name: "arrow_ordinary_right"
            anchors.centerIn: parent
            sourceSize.width: 15
            sourceSize.height: 15
            theme: DTK.themeType
            palette {
                foreground: "#fff"
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                timer.restart()
                pathView.incrementCurrentIndex()
            }
        }
    }
}