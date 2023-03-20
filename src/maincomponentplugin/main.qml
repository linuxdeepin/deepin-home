// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0
import "index" as DIndex
import "./titlebar"
import "./api"

import org.deepin.dtk.impl 1.0 as D

AppLoader {
    id: appLoader
    Component {
        id: main_component
        Loader {
            id: index_loader
            source: "index/Index.qml"
            width: appLoader.width
            height: appLoader.height 
            Connections {
                target: index_loader.item
                ignoreUnknownSignals: true
                // 刷新页面
                function onPageRefresh() {
                    console.log("onRefresh")
                    index_loader.source = "index/Index.qml"
                }
            }
            Connections {
                target: actualTitleBar.item
                // 显示新闻通知
                function onNotifyClicked() {
                    console.log("onNotifyClicked")
                    index_loader.item.showNotifyList()
                }
            }
            Connections {
                target: API
                // 显示断网页面
                function onNetworkError() {
                    index_loader.source = "offline/Offline.qml"
                }
            }
        }
    }
    // 自定义标题栏
    Loader {
        id: actualTitleBar
        asynchronous: true
        sourceComponent: MyTitleBar {}
    }
    Component.onCompleted: {
        // TODO 暂不支持暗色主题，固定为亮色主题
        D.ApplicationHelper.setPaletteType(D.ApplicationHelper.LightType)
        // 设置标题栏
        appLoader.window.header = actualTitleBar
    }
}