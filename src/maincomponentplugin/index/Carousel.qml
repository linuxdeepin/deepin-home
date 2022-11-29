// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later


import QtQuick 2.0
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0


Control {
    id: root
    property ListModel model
    function modelInited() {
        pathView.decrementCurrentIndex()
    }
    Rectangle {
        id: main
        anchors.fill: parent
        visible: false
        Component {
            id: delegate
            Item {
                id: item
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
                startX: -root.width; startY: 0
                PathLine { x: (model.count-1)*root.width; y: 0 }
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
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                let index = pathView.currentIndex + 1
                if (index >= pathView.count) {
                    index = 0
                }
                Qt.openUrlExternally(root.model.get(index).url);
	    }
        }
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
        id: left_arrow_btn
        width: 30
        height: 30
        radius: 15
        color: Qt.rgba(0, 0, 0, 0.1)
        Behavior on color { PropertyAnimation {} }
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
            hoverEnabled: true
            onEntered: {
                left_arrow_btn.color=Qt.rgba(255, 255, 255, 0.3)
            }
            onExited: {
                left_arrow_btn.color=Qt.rgba(0, 0, 0, 0.1)
            }
            onClicked: {
                timer.restart()
                pathView.decrementCurrentIndex()
            }
        }
    }
    Rectangle {
        id: right_arrow_btn
        width: 30
        height: 30
        radius: 15
        color: Qt.rgba(0, 0, 0, 0.1)
        Behavior on color { PropertyAnimation {} }
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
            hoverEnabled: true
            onEntered: {
                right_arrow_btn.color=Qt.rgba(255, 255, 255, 0.3)
            }
            onExited: {
                right_arrow_btn.color=Qt.rgba(0, 0, 0, 0.1)
            }
            onClicked: {
                timer.restart()
                pathView.incrementCurrentIndex()
            }
        }
    }
}
