// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import org.deepin.dtk 1.0
import "../api"
import "../router"

// 标题栏的菜单切换按钮
ButtonBox {
    id: root
    property bool allFeedback: Router.routeCurrent.path === Router.routeAllFeedback.path || Router.routeCurrent.path === Router.routeMyFeedback.path
    property bool accountFeedback: Router.routeCurrent.path === Router.routeFavoriteFeedback.path || Router.routeCurrent.path === Router.routeUrgeFeedback.path
    visible: allFeedback || accountFeedback
    ToolButton {
        visible: root.allFeedback
        id: allFeedbackBtn
        leftPadding: 20
        rightPadding: 20
        implicitHeight: root.height-6
        checked: Router.routeCurrent.path === Router.routeAllFeedback.path
        text: qsTr("Feedback Hub")
        font.weight: Font.Medium
        font.pixelSize: DTK.fontManager.t6.pixelSize
        onClicked: {
            Router.showAllFeedback(true)
        }
    }
    ToolButton {
        visible: root.allFeedback
        id: myFeedbackBtn
        leftPadding: 20
        rightPadding: 20
        implicitHeight: root.height-6
        checked: Router.routeCurrent.path === Router.routeMyFeedback.path
        text: qsTr("My Feedbacks")
        font.weight: Font.Medium
        font.pixelSize: DTK.fontManager.t6.pixelSize
        onClicked: {
            if(!API.isLogin) {
                API.login()
                return
            }
            Router.showMyFeedback(true)
        }
    }
    ToolButton {
        visible: root.accountFeedback
        id: starsFeedbackBtn
        leftPadding: 20
        rightPadding: 20
        implicitHeight: root.height-6
        checked: Router.routeCurrent.path === Router.routeFavoriteFeedback.path
        text: qsTr("My Favorites")
        font.weight: Font.Medium
        font.pixelSize: DTK.fontManager.t6.pixelSize
        onClicked: {
            Router.showFavoriteFeedback(true)
        }
    }

    ToolButton {
        visible: root.accountFeedback
        id: watchFeedbackBtn
        leftPadding: 20
        rightPadding: 20
        implicitHeight: root.height-6
        checked: Router.routeCurrent.path === Router.routeUrgeFeedback.path
        text: qsTr("My Urged")
        font.weight: Font.Medium
        font.pixelSize: DTK.fontManager.t6.pixelSize
        onClicked: {
            Router.showUrgeFeedback(true)
        }
    }
}