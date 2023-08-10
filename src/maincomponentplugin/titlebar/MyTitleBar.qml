// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import org.deepin.dtk.style 1.0 as DS
import "../api"
import "../router"

// 窗口图标和返回按钮
TitleBar {
    id: root
    height: DS.Style.titleBar.height
    icon.name: "deepin-home"
    signal notifyClicked()
    
    leftContent: RowLayout {
        Button {
            Layout.leftMargin: 5
            visible: Router.routeCurrent !== Router.routeIndex
            Layout.maximumWidth: 36
            height: 36
            icon.name: "arrow_ordinary_left"
            icon.width: 7
            icon.height: 12
            onClicked: {
                Router.back()
            }
        }
    }
    embedMode: false
    menu: Menu {
        width: 250
        Action {
            checked: API.autostart
            text: qsTr("Launch at system startup")
            onTriggered: {
                API.setAutoStart(!API.getAutoStart())
            }
        }
        AboutAction {
            text: qsTr("About")
            aboutDialog: root.aboutDialog
        }
        QuitAction {
            text: qsTr("Exit")
            onTriggered: {
                worker.quit()
            }
        }
    }
    
    aboutDialog: AboutDialog {
        width: 400
        modality: Qt.NonModal
        version: Qt.application.version
        productName: qsTr("Deepin Home")
        productIcon: "deepin-home"
        companyLogo: "file://" + DTK.deepinDistributionOrgLogo
        websiteName: DTK.deepinWebsiteName
        websiteLink: DTK.deepinWebsiteLink
        description: qsTr("Quick login to forums, submit suggestions, and report bugs.")
    }

    content: ColumnLayout {
        Rectangle {
            Layout.fillWidth: true
            height: root.height
            TabButton {
                anchors.centerIn: parent
                height: 36
            }
            AccountButton {
                anchors.right: notifyButton.left
                width: parent.height
                height: parent.height
            }
            NotifyButton {
                id: notifyButton
                width: parent.height
                height: parent.height
                anchors.right: parent.right
                onClicked: {
                    root.notifyClicked()
                }
            }
        }
    }
    // 阴影
    BoxShadow {
        anchors.fill: parent
        shadowBlur : 10
        shadowColor : Qt.rgba(0,0,0,0.03)
        shadowOffsetX : 0
        shadowOffsetY : 4
        hollow: true
    }
}
