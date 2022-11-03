// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0
import "index" as DIndex
import "./titlebar"

AppLoader {
    id: appLoader
    property int listIndex: -1
    Component {
        Rectangle {
            DIndex.Index {
                id: index
                listIndex: appLoader.listIndex
                width: appLoader.width
                height: appLoader.height 
                onQuestionnaireClicked: {
                    console.log("onNotifyClicked")
                    appLoader.listIndex = 1
                }
                onQuestionnaireCancel: {
                    appLoader.listIndex = -1
                }
            }
        }
    }

    Loader {
        id: actualTitleBar
        height: 50
        asynchronous: true
        sourceComponent: MyTitleBar {
            onNotifyClicked: {
                console.log("onNotifyClicked")
                appLoader.listIndex = 0
            }
        }
    }

    Component.onCompleted: {
        appLoader.window.header = actualTitleBar
    }
}