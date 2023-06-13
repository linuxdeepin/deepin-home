// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import QtGraphicalEffects 1.0
import org.deepin.dtk 1.0
import QtQuick.Dialogs 1.0
import "../api"
import "../router"


Item {
    id: root
    // 对话框窗口关闭后发出信号
    signal closed()
    // 反馈类型
    property int type: 0
    // 表单图片列表
    property ListModel imgListModel: ListModel {}

    // 追加表单图片
    function appendImage(src) {
        if(imgListModel.count >= 3){
            return
        }
        const info = API.getFileInfo(src)
        if(info.size > 1024*1024) {
            API.notify(qsTr("Unable to add a screenshot."), qsTr("The image file size should be less than 1MB."))
            return
        }
        let allow=false
        for(const ext of [".png", ".jpg", ".gif"]){
            if(info.filename.endsWith(ext)){
                allow=true
            }
        }
        if(!allow){
            API.notify(qsTr("Unable to add a screenshot."), qsTr("The image file format is not supported for uploading."))
            return
        }
        imgListModel.append({"source": src})
    }
    DialogWindow {
        id: win
        icon: "deepin-home"
        title: qsTr("Submit Feedback")
        width: root.width*0.8
        height: submitLayout.height + 100
        // 表单控件宽度
        property int controlWidth: width - 100 * 2

        onClosing: {
            root.closed()
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
            id: submitLayout
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
                    id: titleText
                    width: win.controlWidth
                    selectByMouse: true
                    text: ""
                    onTextChanged: {
                        if (length > 60) {
                            remove(60, length)
                        }
                    }
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
                        height: 140
                        TextArea {
                            id: contentText
                            selectByMouse: true
                            wrapMode: Text.Wrap
                            onTextChanged: {
                                if (length > 1000) {
                                    remove(1000, length)
                                }
                            }
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
                    id: emailText
                    selectByMouse: true
                    validator: RegExpValidator { regExp: /^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$/ }
                    width: win.controlWidth
                    text: ""
                }
            }

            // 系统版本
            Row {
                spacing: 10
                ControlLabel {
                    text: qsTr("System Version:")
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: versionText
                    selectByMouse: true
                    width: win.controlWidth
                    readOnly: true
                    text: API.sysVersion()
                }
            }
            // 上传设备信息
            // TODO 功能暂时不做
            Row {
                visible: false
                spacing: 10
                ControlLabel {
                    text: qsTr("Device Info：")
                    verticalAlignment: Text.AlignVCenter
                }
                Row {
                    Switch {
                        height: reqType.height
                        checked: true
                    }
                    Text {
                        visible: bugType.checked
                        id: sysinfoText
                        text: qsTr("Upload <a href='home://sysinfo'>device information</a>")
                        linkColor: "blue"
                        height: reqType.height
                        verticalAlignment: Text.AlignVCenter
                        onLinkActivated: {
                            sysinfoView.visible = true
                        }
                    }
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
                            nameFilters: [qsTr("Image files")+" (*.png *.jpg *.gif)"]
                            onAccepted: {
                                for(const f of fileDialog.fileUrls){
                                    root.appendImage(f)
                                }
                            }
                        }
                        // 已添加图片
                        ListView {
                            width: 58 * model.count - 10;
                            height: 48;
                            id: imgList
                            orientation: ListView.Horizontal
                            model: root.imgListModel
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
                                    onStatusChanged: {
                                        if (status == Image.Error) {
                                            API.notify(qsTr("Unable to add a screenshot."), qsTr("The image file format is not supported for uploading."))
                                            imgList.model.remove(index)
                                        }
                                    }
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
                            visible: root.imgListModel.count < 3
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
            // 底部按钮
            Row {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter
                DialogWindow {
                    id: cancelConfirm
                    width: cancelText.width+80
                    height: cancelColumn.height+80
                    ColumnLayout {
                        id: cancelColumn
                        width: parent.width
                        spacing: 20
                        Text {
                            id: cancelText
                            Layout.alignment: Qt.AlignHCenter
                            font: DTK.fontManager.t5
                            text: qsTr("Are you sure you want to exit the feedback submission?")
                        }
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            font: DTK.fontManager.t7
                            text: qsTr("The feedback content will not be saved.")
                        }
                        Row {
                            Layout.alignment: Qt.AlignHCenter
                            spacing: 10
                            Button {
                                text: qsTr("Cancel")
                                onClicked: {
                                    cancelConfirm.close()
                                }
                            }
                            WarningButton {
                                text: qsTr("Exit")
                                onClicked: {
                                    cancelConfirm.close()
                                    win.close()
                                }
                            }
                        }
                        // 增加底部间距
                        Row {
                        }
                    }
                }
                Button {
                    width: 200
                    text: qsTr("Cancel")
                    onClicked: {
                        // 如果已填写标题或内容，弹出提示
                        if(titleText.text.length>0 || contentText.text.length>0){
                            cancelConfirm.show()
                        } else {
                            win.close()
                        }
                    }
                }
                RecommandButton {
                    width: 200
                    text: qsTr("Submit")
                    onClicked: {
                        if(titleText.text.length==0){
                            API.notify(qsTr("Unable to submit feedback."), qsTr("Please provide the title of your feedback."))
                            return
                        }
                        if(contentText.text.length==0){
                            API.notify(qsTr("Unable to submit feedback."), qsTr("Please provide the content of your feedback."))
                            return
                        }
                        if(emailText.text.length>0 && !emailText.acceptableInput){
                            API.notify(qsTr("Unable to submit feedback."), qsTr("Incorrect email address entered."))
                            return
                        }
                        if(!API.isLogin){
                            API.login()
                            return
                        }
                        let screenshots = []
                        const submit = () => {
                            API.getLanguage(lang=>{
                                API.createFeedback({
                                    type: bugType.checked ? "bug" : "req",
                                    language: lang,
                                    title: titleText.text,
                                    content: contentText.text,
                                    email: emailText.text,
                                    version: versionText.text,
                                    screenshots: screenshots,
                                }, () => {
                                    Router.showMyFeedback()
                                })
                            })
                        }
                        if(root.imgListModel.count == 0){
                            submit()
                            return
                        }
                        const uploadCallback = (id, index) => {
                            screenshots.push(id)
                            if(screenshots.length===root.imgListModel.count) {
                                submit()
                            }
                        }
                        for(var i = 0; i < root.imgListModel.count; i++) {
                            const item = root.imgListModel.get(i)
                            API.upload(item.source, (id)=>{
                                uploadCallback(id, i)
                            })
                        }
                    }
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
                    root.appendImage(f)
                }
            }
        }
        // 设备信息
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