// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0
import "index" as DIndex
import "./titlebar"

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
            onLoaded: {
                if(index_loader.status == Loader.Null) {
                    return
                }
                index_loader.item.listIndex = -1
            }
            Connections {
                target: index_loader.item
                function onQuestionnaireClicked() {
                    console.log("onQuestionnaireClicked")
                    index_loader.item.listIndex = 1
                }
                function onListHide() {
                    console.log("onListHide")
                    index_loader.item.listIndex = -1
                }
                function onPageRefresh() {
                    console.log("onRefresh")
                    const tmp = index_loader.source
                    index_loader.source = ""
                    index_loader.source = tmp
                }
            }
            Connections {
                target: actualTitleBar.item
                function onNotifyClicked() {
                    console.log("onNotifyClicked")
                    index_loader.item.listIndex = 0
                }
            }
        }
    }
    Loader {
        id: actualTitleBar
        asynchronous: true
        sourceComponent: MyTitleBar {}
    }

    Component.onCompleted: {
        D.ApplicationHelper.setPaletteType(D.ApplicationHelper.LightType)
        appLoader.window.header = actualTitleBar
    }
}