// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import org.deepin.dtk.style 1.0 as DS
import "../api"

TitleBar {
    id: root
    height: DS.Style.titleBar.height
    signal notifyClicked()
    leftContent: Image {
        source: "/images/deepin-home.svg"
        sourceSize.width:  root.height - 10
        sourceSize.height: root.height - 10
        x: 10
        anchors.verticalCenter: parent.verticalCenter
    }
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
        version: qsTr("Version: %1").arg(Qt.application.version)
        productName: qsTr("Deepin Home")
        // productIcon: "deepin-home"
        companyLogo: "file://" + DTK.deepinDistributionOrgLogo
        websiteName: DTK.deepinWebsiteName
        websiteLink: DTK.deepinWebsiteLink

        IconLabel {
            x: 5
            anchors.horizontalCenter: parent.horizontalCenter
            icon.name: "deepin-home"
            icon.width: 96
            icon.height: 96
        }
    }

    content: RowLayout {
        anchors.fill: parent
        // 填充空白
        Item {
            Layout.fillWidth: true
        }
        // 账户相关
        Rectangle {
            id: account_item
            Layout.preferredWidth: 110
            Layout.preferredHeight: parent.height
            // 头像
            Image {
                id: avatar_image
                source: API.isLogin ? API.avatar : "/images/avatar.svg"
                sourceSize.width: 26
                sourceSize.height: 26
                anchors.verticalCenter: parent.verticalCenter
            }
            // 昵称
            Text {
                text: API.isLogin ? API.nickname : qsTr("Unlogged")
                id: nickname_text
                width: 60
                elide: Text.ElideRight
                anchors.left: avatar_image.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            // 下拉箭头
            DciIcon {
                name: "arrow_ordinary_down"
                sourceSize.width: 12
                sourceSize.height: 12
                theme: DTK.themeType
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
            // 鼠标点击
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked:{
                    var pos = Qt.point(avatar_image.x, root.height-12)
                    if(API.isLogin) {
                        accountMenu.popup(avatar_image, pos)
                    } else {
                        loginMenu.popup(avatar_image, pos)
                    }
                }
            }
            // 未登录菜单
            Menu {
                id: loginMenu
                width: account_item.width
                maxVisibleItems: 10
                MenuItem { 
                    text: qsTr("Sign in") 
                    onTriggered: {
                        worker.login()
                    }
                }
            }
            // 已登录菜单
            Menu {
                id: accountMenu
                width: account_item.width
                maxVisibleItems: 10
                MenuItem {
                    text: qsTr("Sign out") 
                    onTriggered: {
                        worker.logout()
                    }
                }
            }
        }
        // 通知图标
        WindowButton {
            property bool hover
            Image {
                id: notify_icon
                source: "/images/msg.svg"
                sourceSize.width: 18
                sourceSize.height: 14
                visible: false
            }
            ColorOverlay {
                id: button_icon
                source: notify_icon
                width: 18
                height: 14
                anchors.centerIn: parent
                color: parent.hover ? "#0081FF" : "black";
                transform: rotation
                antialiasing: true
            }
            Rectangle {
                color: "red"
                visible: API.msgCount > 0
                anchors.left: button_icon.right
                anchors.leftMargin: -6
                anchors.bottom: button_icon.top
                anchors.bottomMargin: -6
                width: count_text.width + 6
                height: count_text.height
                radius: 5
                Text {
                    id: count_text
                    text: API.msgCount
                    color: "#fff"
                    anchors.centerIn: parent
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    parent.hover = true
                }
                onExited: {
                    parent.hover = false
                }
                onClicked: {
                    root.notifyClicked()
                }
            }
        }
    }
    embedMode: false
}
