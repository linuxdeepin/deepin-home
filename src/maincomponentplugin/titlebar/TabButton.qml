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
        checked: Router.routeCurrent.path === Router.routeAllFeedback.path
        text: qsTr("Feedback Hub")
        onClicked: {
            Router.showAllFeedback(false)
        }
    }
    ToolButton {
        visible: root.allFeedback
        id: myFeedbackBtn
        checked: Router.routeCurrent.path === Router.routeMyFeedback.path
        text: qsTr("My Feedbacks")
        onClicked: {
            Router.showMyFeedback(false)
        }
    }

    ToolButton {
        visible: root.accountFeedback
        id: starsFeedbackBtn
        checked: Router.routeCurrent.path === Router.routeFavoriteFeedback.path
        text: qsTr("My Favorites")
        onClicked: {
            Router.showFavoriteFeedback(false)
        }
    }

    ToolButton {
        visible: root.accountFeedback
        id: watchFeedbackBtn
        checked: Router.routeCurrent.path === Router.routeUrgeFeedback.path
        text: qsTr("My Urge")
        onClicked: {
            Router.showUrgeFeedback(false)
        }
    }
}