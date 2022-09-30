// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later


import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import QtGraphicalEffects 1.0
import org.deepin.dtk 1.0

Control {
    id: root
    property bool disabled
    property string title
    property string subtitle
    property string icon

    Rectangle {
        id: boxShadowSource
        width: root.width
        height: root.height
        radius: 20
        gradient: Gradient {
            GradientStop {  position: 0.0;    color: "#F4F6FD"  }
            GradientStop {  position: 1.0;    color: "#FEFEFF" }
        }
        // 左边的图标
        Image {
            id: icon
            x: 15
            height: root.height-30
            anchors.verticalCenter: parent.verticalCenter
            source: root.icon
            sourceSize.height: height
        }
        // 中间的内容
        Rectangle {
            width: root.width-15-icon.width
            anchors.left: icon.right
            anchors.top: icon.top
            anchors.bottom: icon.bottom
            color: "transparent"
            Text {
                id: title
                font.pixelSize: 20
                font.bold: true
                text: root.title
                anchors.bottom: parent.verticalCenter
            }
            Text {
                font.pointSize: 12
                color: "#000"
                opacity: 0.6
                anchors.top: title.bottom
                anchors.topMargin: 4
                text: root.subtitle
            }

            Rectangle {
                visible: disabled
                anchors.verticalCenter: title.verticalCenter
                anchors.left: title.right
                anchors.leftMargin: 6
                id: roundRect
                radius: 14
                color: "black"
                width: 60
                height: 24
                Rectangle {
                    id: squareRect
                    color: "red"
                    width: 13
                    height: 13
                    radius: 4
                    anchors.top: roundRect.top
                    anchors.left: roundRect.left
                }
                LinearGradient {
                    source: parent
                    anchors.fill: parent
                    start: Qt.point(0, height/3)
                    end: Qt.point(width, height/3)
                    gradient: Gradient {
                        GradientStop {  position: 0.12;    color: "#ffa469"  }
                        GradientStop {  position: 0.76;    color: "#fe3232" }
                    }
                }
                Text {
                    text: "敬请期待"
                    color: "#fff"
                    anchors.centerIn: parent
                }
            }
        }
        // 右边箭头 
        Control {
            id: control
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            width: 12
            height: 12
            DciIcon {
                name: "menu_arrow"
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                theme: DTK.themeType
            }
        }
    }
    // 阴影
    BoxShadow {
        anchors.fill: boxShadowSource
        shadowBlur : 12
        shadowColor : Qt.rgba(0.07, 0.2, 0.5, 0.05)
        shadowOffsetX : 0
        shadowOffsetY : 5
        cornerRadius: boxShadowSource.radius
        hollow: true
    }
}