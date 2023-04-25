// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import QtGraphicalEffects 1.0
import org.deepin.dtk 1.0
import "../api"
import "../widgets"

Item {
    id: root
    property int index
    Popup {
        id: popup
        x: 80
        y: 50
        width: parent.width-x*2
        height: parent.height-y*2
        Loading {
            id: loading
            visible: true
        }
        Component.onCompleted: {
            open()
        }
        Item {
            anchors.fill: parent
            // 延迟 200ms 加载内容，避免加载动画闪烁
            Timer {
                id: timer
                interval: 200
                running: true
                onTriggered: {
                    if (root.index === 1) {
                        API.getInternalTest((resp) => {
                            content.text = resp.value
                            loading.visible = false
                        })
                    } else {
                        API.getAboutUs((resp) => {
                            content.text = resp.value
                            loading.visible = false
                        })
                    }
                }
            }
            // 关闭按钮
            WindowButton {
                id: btn
                anchors.right: parent.right
                width: 40
                height: 40
                icon.name: "window_close"
                icon.width: width
                icon.height: height
                onClicked: {
                    popup.close()
                }
            }
            // 内容
            ScrollView {
                anchors.top: btn.bottom
                anchors.bottom: parent.bottom
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                width: parent.width
                clip: true
                Column {
                    Text {
                        id: content
                        x: 30
                        width: popup.width - 30*2
                        wrapMode: Text.Wrap
                        textFormat: Text.RichText
                        onLinkActivated: (link) => {
                            Qt.openUrlExternally(link)
                        }
                    }
                }
            }
        }
    }
}