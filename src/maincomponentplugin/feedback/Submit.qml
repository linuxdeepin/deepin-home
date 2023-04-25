// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import QtGraphicalEffects 1.0
import org.deepin.dtk 1.0
import QtQuick.Dialogs 1.0

Item {
    id: root
    property int type: 0
    DialogWindow {
        id: win
        icon: "deepin-home"
        title: qsTr("Submit Feedback")
        width: root.width*0.8
        height: root.height*0.8
        // 表单控件宽度
        property int controlWidth: width - 100 * 2
        // 表单图片列表
        property ListModel imgListModel: ListModel {}
        // 追加表单图片
        function appendImage(src) {
            if(imgListModel.count >= 3){
                return
            }
            imgListModel.append({"source": src})
        }
        component ControlLabel: Text {
            font: DTK.fontManager.t6
            width: 100
            height: parent.height
            horizontalAlignment: Text.AlignRight
        }
        // 默认显示出窗口
        Component.onCompleted: {
            show()
            if(type == 0){
                bugType.checked = true
            } else {
                reqType.checked = true
            } 
        }
        // 提交表单
        ColumnLayout {
            width: parent.width
            spacing: 10
            Label {
                Layout.alignment: Qt.AlignHCenter
                font: DTK.fontManager.t5
                text: win.title
            }
            // 反馈类型
            Row {
                spacing: 10
                ControlLabel {
                    text: qsTr("Type：")
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle {
                    color: Qt.rgba(0,0,0,0.08);
                    height: reqType.height
                    width: win.controlWidth
                    radius: 8
                    RadioButton {
                        id: reqType
                        checked: true
                        width: 150 // 控件宽度没自适应
                        text: qsTr("Suggestions")
                    }
                    RadioButton {
                        id: bugType
                        width: 150 // 控件宽度没自适应
                        anchors.left: reqType.right
                        text: qsTr("Bug Report")
                    }
                    Switch {
                        visible: bugType.checked
                        height: reqType.height
                        checked: true
                        anchors.right: sysinfoText.left
                    }
                    Text {
                        visible: bugType.checked
                        id: sysinfoText
                        text: qsTr("Upload <a href='home://sysinfo'>device information</a>")
                        linkColor: "blue"
                        height: reqType.height
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        onLinkActivated: {
                            sysinfoView.visible = true
                        }
                    }
                }
            }
            // 标题
            Row {
                spacing: 10
                ControlLabel {
                    text: qsTr("Title：")
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    width: win.controlWidth
                    text: ""
                }
            }
            // 内容
            Row {
                spacing: 10
                ControlLabel {
                    text: qsTr("Content：")
                }
                Column {
                    ScrollView {
                        width: win.controlWidth
                        height: 180
                        TextArea {
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Label {
                        font: DTK.fontManager.t8
                        text: qsTr("Do not fill in multiple requirements in one feedback")
                    }
                }
            }
            // 联系邮箱
            Row {
                spacing: 10
                ControlLabel {
                    text: qsTr("Email：")
                    verticalAlignment: Text.AlignVCenter
                }
                LineEdit {
                    width: win.controlWidth
                    text: ""
                }
            }
            // 图片说明
            Row {
                spacing: 10
                ControlLabel {
                    text: qsTr("Screenshots：")
                }
                Column {
                    spacing: 10
                    Row {
                        spacing: 10
                        // 选择图片对话框
                        FileDialog {
                            id: fileDialog
                            title: "Please choose a file"
                            folder: shortcuts.home
                            selectMultiple: true
                            onAccepted: {
                                for(const f of fileDialog.fileUrls){
                                    win.appendImage(f)
                                }
                            }
                        }
                        // 已添加图片
                        ListView {
                            width: 58 * model.count - 10;
                            height: 48;
                            id: imgList
                            orientation: ListView.Horizontal
                            model: win.imgListModel
                            spacing: 10
                            delegate: Rectangle {
                                color: Qt.rgba(0,0,0,0.08);
                                height: imgList.height
                                width: height
                                radius: 5
                                Image {
                                    anchors.fill: parent
                                    fillMode: Image.PreserveAspectFit
                                    source: model.source
                                }
                                // 鼠标点击移除已添加的图片
                                MouseArea {
                                    Button {
                                        id: removeBtn
                                        anchors.fill: parent
                                        visible: false
                                        icon.name: "action_delete"
                                        icon.width: 24
                                        icon.height: 24 
                                        onClicked: {
                                            imgList.model.remove(index)
                                        }
                                    }
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onEntered: {
                                        removeBtn.visible = true
                                    }
                                    onExited: {
                                        removeBtn.visible = false
                                    }
                                }
                            }
                        }
                        // 添加图片的按钮
                        Button {
                            visible: win.imgListModel.count < 3
                            width: 48
                            height: 48
                            icon.name: "action_add"
                            icon.width: 14
                            icon.height: 14 
                            onClicked: {
                                fileDialog.visible = true
                            }
                        }
                    }
                    Label {
                        font: DTK.fontManager.t8
                        text: qsTr("Drag and drop files or click buttons to add pictures, up to three")
                    }
                }
            }
            // 提交按钮
            Row {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter
                Button {
                    width: 200
                    text: qsTr("Cancel")
                    onClicked: {
                        win.close()
                    }
                }
                RecommandButton {
                    width: 200
                    text: qsTr("Submit")
                }
            }
        }

        // 拖拽文件提示
        Rectangle {
            id: dropRect
            anchors.fill: parent
            visible: false
            color: Qt.rgba(0,0,0,0.08);
        }
        // 拖拽文件处理
        DropArea {
            anchors.fill: parent
            onEntered: {
                dropRect.visible = true
                drag.accept(Qt.CopyAction);
            }
            onExited: {
                dropRect.visible = false
            }
            onDropped: {
                dropRect.visible = false
                for(const f of drop.urls){
                    win.appendImage(f)
                }
            }
        }
        Rectangle {
            id: sysinfoView
            visible: false
            anchors.fill: parent
            Label {
                y: 10
                text: qsTr("Device Information")
                font: DTK.fontManager.t5
                anchors.horizontalCenter: sysinfoView.horizontalCenter
            }
            ScrollView {
                width: win.width*0.8
                height: win.height*0.70
                anchors.centerIn: parent
                TextArea {
                    selectByMouse: true
                    readOnly: true
                    wrapMode: Text.Wrap
                    text: "hello"
                }
            }
            Button {
                width: 200
                text: qsTr("Cancel")
                anchors.bottom: sysinfoView.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: sysinfoView.horizontalCenter
                onClicked: {
                    sysinfoView.visible = false
                }
            }
        }
    }
}