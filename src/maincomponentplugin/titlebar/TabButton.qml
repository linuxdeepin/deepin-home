// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import org.deepin.dtk 1.0
import "../api"
import "../router"

// 标题栏的菜单切换按钮
ButtonBox {
    id: root
    property bool allFeedback: Router.routeCurrent === Router.routeAllFeedback || Router.routeCurrent === Router.routeMyFeedback
    property bool accountFeedback: Router.routeCurrent === Router.routeStarsFeedback || Router.routeCurrent === Router.routeWatchFeedback
    visible: allFeedback || accountFeedback
    ToolButton {
        visible: root.allFeedback
        id: allFeedbackBtn
        checked: Router.routeCurrent === Router.routeAllFeedback
        text: qsTr("Feedback Hub")
        onClicked: {
            Router.showAllFeedback(false)
        }
    }
    ToolButton {
        visible: root.allFeedback
        id: myFeedbackBtn
        checked: Router.routeCurrent === Router.routeMyFeedback
        text: qsTr("My Feedbacks")
        onClicked: {
            Router.showMyFeedback(false)
        }
    }

    ToolButton {
        visible: root.accountFeedback
        id: starsFeedbackBtn
        checked: Router.routeCurrent === Router.routeStarsFeedback
        text: qsTr("My Favorites")
        onClicked: {
            Router.showStarsFeedback(false)
        }
    }

    ToolButton {
        visible: root.accountFeedback
        id: watchFeedbackBtn
        checked: Router.routeCurrent === Router.routeWatchFeedback
        text: qsTr("My Urge")
        onClicked: {
            Router.showWatchFeedback(false)
        }
    }
}