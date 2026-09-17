// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later
// 反馈列表筛选组件
// 在反馈广场使用

import "../router"
import APIProxy 1.0
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.deepin.dtk 1.0

GridLayout {
    id: root

    property var controls: ["keyword", "type", "version", "order"]
    property var value

    signal valueChange(var val)

    function activated() {
        valueChange({
            "type": typeBox.model.get(typeBox.currentIndex).value,
            "order": orderBox.model.get(orderBox.currentIndex).value,
            "version": versionBox.model.get(versionBox.currentIndex).value,
            "keyword": keywordEdit.text
        });
    }

    columns: width >= 1000 ? 4 : width >= 520 ? 2 : 1
    columnSpacing: 10
    rowSpacing: 10
    Component.onCompleted: {
        api.getSystemVersion();
        if (!value)
            return ;

        if (value["type"] == "bug")
            typeBox.currentIndex = 1;
        else if (value["type"] == "req")
            typeBox.currentIndex = 2;
    }

    APIProxy {
        id: api

        onSignalGetSystemVersionResp: (systemVersion) => {
            console.log("systemVersion");
            for (const sv of systemVersion) {
                versionBox.model.append({
                    "text": sv.text,
                    "value": sv.value
                });
            }
        }
    }

    LineEdit {
        id: keywordEdit

        visible: controls.includes("keyword")
        Layout.preferredWidth: Math.min(300, root.width)
        Keys.onReturnPressed: {
            root.activated();
        }

        Text {
            id: searchText

            anchors.centerIn: parent
            visible: !(keywordEdit.focus || keywordEdit.text != "")
            text: qsTr("Search")
        }

        ActionButton {
            icon.name: "search"
            anchors.right: searchText.left
            anchors.top: searchText.top
            visible: !(keywordEdit.focus || keywordEdit.text != "")
            onClicked: {
                keywordEdit.focus = true;
            }
        }

        // 点击清空按钮时
        Connections {
            function onClicked() {
                if (root.value["keyword"])
                    root.activated();

            }

            target: keywordEdit.clearButton.item
        }

    }

    Row {
        visible: controls.includes("type")
        spacing: 10

        Text {
            text: qsTr("Type:")
            anchors.verticalCenter: parent.verticalCenter
        }

        ComboBox {
            id: typeBox

            width: 140
            textRole: "text"
            HoverHandler { id: typeHover }
            TextMetrics {
                id: typeTextMetrics
                font: typeBox.font
                text: typeBox.displayText
            }
            ToolTip.visible: typeHover.hovered && contentItem
                             && typeTextMetrics.width > contentItem.width
            ToolTip.text: displayText
            ToolTip.delay: 300
            onActivated: {
                root.activated();
            }

            model: ListModel {
                ListElement {
                    text: qsTr("All")
                    value: ""
                }

                ListElement {
                    text: qsTr("Bug")
                    value: "bug"
                }

                ListElement {
                    text: qsTr("Suggestions")
                    value: "req"
                }

            }

        }
    }

    Row {
        visible: controls.includes("version")
        spacing: 10

        Text {
            text: qsTr("Version:")
            anchors.verticalCenter: parent.verticalCenter
        }

        ComboBox {
            id: versionBox

            textRole: "text"
            width: 120
            HoverHandler { id: versionHover }
            TextMetrics {
                id: versionTextMetrics
                font: versionBox.font
                text: versionBox.displayText
            }
            ToolTip.visible: versionHover.hovered && contentItem
                             && versionTextMetrics.width > contentItem.width
            ToolTip.text: displayText
            ToolTip.delay: 300
            onActivated: {
                root.activated();
            }

            model: ListModel {
                ListElement {
                    text: qsTr("All")
                    value: ""
                }

            }

        }
    }

    Row {
        visible: controls.includes("order")
        spacing: 10

        Text {
            text: qsTr("Sort:")
            anchors.verticalCenter: parent.verticalCenter
        }

        ComboBox {
            id: orderBox

            textRole: "text"
            width: 120
            HoverHandler { id: orderHover }
            TextMetrics {
                id: orderTextMetrics
                font: orderBox.font
                text: orderBox.displayText
            }
            ToolTip.visible: orderHover.hovered && contentItem
                             && orderTextMetrics.width > contentItem.width
            ToolTip.text: displayText
            ToolTip.delay: 300
            onActivated: {
                root.activated();
            }

            model: ListModel {
                ListElement {
                    text: qsTr("Newest")
                    value: ""
                }

                ListElement {
                    text: qsTr("Favorite")
                    value: "collect"
                }

                ListElement {
                    text: qsTr("Urge")
                    value: "like"
                }

            }

        }
    }

}
