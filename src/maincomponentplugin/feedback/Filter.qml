// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later
// 反馈列表筛选组件
// 在反馈广场使用

import "../router"
import APIProxy 1.0
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0

RowLayout {
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

    spacing: 10
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
        Layout.preferredWidth: 300
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

    Item {
        Layout.fillWidth: true
    }

    Text {
        visible: controls.includes("type")
        Layout.leftMargin: 30
        text: qsTr("Type:")
    }

    ComboBox {
        id: typeBox

        visible: controls.includes("type")
        Layout.preferredWidth: 140
        textRole: "text"
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

    Text {
        visible: controls.includes("version")
        Layout.leftMargin: 30
        text: qsTr("Version:")
    }

    ComboBox {
        id: versionBox

        visible: controls.includes("version")
        textRole: "text"
        Layout.preferredWidth: 120
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

    Text {
        visible: controls.includes("order")
        Layout.leftMargin: 30
        text: qsTr("Sort:")
    }

    ComboBox {
        id: orderBox

        visible: controls.includes("order")
        textRole: "text"
        Layout.preferredWidth: 120
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
