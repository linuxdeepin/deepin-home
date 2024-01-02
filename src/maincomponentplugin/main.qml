// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later

import "./api"
import "./router"
import "./titlebar"
import QtQuick 2.11
import org.deepin.dtk 1.0
import org.deepin.dtk.impl 1.0 as D

AppLoader {
    id: appLoader

    Component.onCompleted: {
        // TODO 暂不支持暗色主题，固定为亮色主题
        D.ApplicationHelper.setPaletteType(D.ApplicationHelper.LightType);
        // 设置标题栏
        appLoader.window.header = actualTitleBar;
        for (let i = 1; i <= 10; i++) {
            console.log("font_size_t" + i, DTK.fontManager["t" + i].pixelSize);
        }
    }

    Component {
        id: main_component

        Rectangle {
            width: appLoader.width
            height: appLoader.height

            // 通过路由控制，多个页面叠加显示
            Repeater {
                id: pages

                // 使用Rectangle做背景
                Rectangle {
                    required property int index

                    anchors.fill: parent
                    Component.onCompleted: {
                        const m = pages.model.get(index);
                        if (m.data)
                            pageLoader.setSource(m.source, m.data);
                        else
                            pageLoader.setSource(m.source);
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                    }

                    Loader {
                        id: pageLoader

                        anchors.fill: parent
                    }

                }

                model: ListModel {
                    ListElement {
                        source: "index/Index.qml"
                    }

                }

            }

            Connections {
                // 路由导航，添加一个页面
                function onSignalNavigate(route, overlay) {
                    if (overlay)
                        pages.model.remove(pages.count - 1);

                    pages.model.append({
                        "source": route.component,
                        "data": route.data
                    });
                }

                // 路由反馈，删除一个页面
                function onSignalBack() {
                    pages.model.remove(pages.count - 1);
                    console.log(pages.count);
                }

                // 回到首页，清空多余页面，释放内存
                function onSignalGoHome(route) {
                    console.log("go home");
                    pages.model.clear();
                    pages.model.append({
                        "source": route.component,
                        "data": route.data
                    });
                }

                target: Router
            }

            Connections {
                // 菜单栏的通知按钮点击事件，由于通知侧边栏在首页组件中所以先导航到首页
                function onNotifyClicked() {
                    console.log("onNotifyClicked");
                    Router.showNotify();
                }

                target: actualTitleBar.item
            }

            Connections {
                // 显示断网页面
                function onNetworkError() {
                    Router.showNetworkError();
                }

                // 托盘激活窗口
                function onShowMainWindow(isIconClick) {
                    // 关闭窗口特效的环境需要先恢复窗口才能激活
                    window.showNormal();
                    if (window.active) {
                        // 如果窗口已激活，点击托盘将关闭窗口
                        if (isIconClick) {
                            console.log("close window");
                            window.close();
                        }
                    } else {
                        console.log("active window");
                        window.requestActivate();
                    }
                }

                target: API
            }

        }

    }
    // 自定义标题栏

    Loader {
        id: actualTitleBar

        asynchronous: true

        sourceComponent: MyTitleBar {
        }

    }

}
