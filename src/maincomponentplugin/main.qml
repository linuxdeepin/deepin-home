// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0
import "index" as DIndex
import "./titlebar"
import "./api"
import "./router"

import org.deepin.dtk.impl 1.0 as D

AppLoader {
    id: appLoader
    Component {
        id: main_component
        Loader {
            id: index_loader
            width: appLoader.width
            height: appLoader.height
            source: "index/Index.qml"
            Connections {
                target: actualTitleBar.item
                // 菜单栏的通知按钮点击事件，由于通知侧边栏在首页组件中所以先导航到首页
                function onNotifyClicked() {
                    console.log("onNotifyClicked")
                    Router.showIndex()
                    index_loader.item.showNotifyList()
                }
            }
            Connections {
                target: Router
                // 路由更改事件
                function onRouteCurrentChanged() {
                    if (Router.routeCurrent.data) {
                        index_loader.setSource(Router.routeCurrent.component, Router.routeCurrent.data)
                    } else {
                        index_loader.setSource(Router.routeCurrent.component)
                    }
                }
            }
            Connections {
                target: API
                // 显示断网页面
                function onNetworkError() {
                    Router.showNetworkError()
                }
                // 托盘激活窗口
                function onShowMainWindow(isIconClick) {
                    // 关闭窗口特效的环境需要先恢复窗口才能激活
                    window.showNormal()

                    if (window.active) {
                        // 如果窗口已激活，点击托盘将关闭窗口
                        if(isIconClick) {
                            window.close()
                        }
                    } else {
                        window.requestActivate()
                    }
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