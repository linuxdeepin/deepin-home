// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import QtGraphicalEffects 1.0
import org.deepin.dtk 1.0
import "../api"
import "../widgets"

Control {
    id: root
    property int index
    signal canceled()
    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(0,0,0,0.2)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.canceled()
            }
        }
    }
    InWindowBlur {
        id: blur
        anchors.fill: dialog
        radius: 20
        offscreen: false
    }
    Rectangle {
        id: dialog
        x: 80
        y: 50
        radius: 20
        width: parent.width-x*2
        height: parent.height-y*2
        color: Qt.rgba(1,1,1,0.8)
        MouseArea {
            anchors.fill: parent
        }
        Button {
            id: btn
            x: parent.width-50
            y: 10
            width: 36
            height: 36
            icon.name: "window_close"
            icon.width: 36
            icon.height: 36
            onClicked: {
                console.log("click")
                root.canceled()
            }
        }
        ScrollView {
            anchors.top: btn.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            width: parent.width
            clip: true
            Column {
                Text {
                    id: content
                    x: 30
                    y: 30
                    width: dialog.width-x*2
                    wrapMode: Text.Wrap
                    textFormat: Text.RichText
                    onLinkActivated: (link) => {
                        Qt.openUrlExternally(link)
                    }
                }
            }
        }
    }
    Loading {
        visible: content.text == ""
    }
    Component.onCompleted: {
        if (root.index === 1) {
            API.getInternalTest((resp)=>{
                content.text = resp.value
            })
        } else {
            API.getAboutUs((resp)=>{
                content.text = resp.value
            })
        }
    }
}