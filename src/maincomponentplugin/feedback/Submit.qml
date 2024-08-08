// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later

import "../api"
import "../router"
import APIProxy 1.0
import QtGraphicalEffects 1.0
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0

Item {
    id: root

    // 反馈类型
    property string type: "req"
    // 表单图片列表
    property ListModel imgListModel

    // 对话框窗口关闭后发出信号
    signal closed()

    // 追加表单图片
    function appendImage(src) {
        if (imgListModel.count >= 3)
            return ;

        const info = API.getFileInfo(src);
        if (info.size > 1024 * 1024) {
            API.notify(qsTr("Unable to add a screenshot."), qsTr("The image file size should be less than 1MB."));
            return ;
        }
        let allow = false;
        for (const ext of [".png", ".jpg", ".gif"]) {
            if (info.filename.endsWith(ext))
                allow = true;

        }
        if (!allow) {
            API.notify(qsTr("Unable to add a screenshot."), qsTr("The image file format is not supported for uploading."));
            return ;
        }
        api.uploadFile(src);
    }

    APIProxy {
        id: api

        onSignalUploadFileResp: (filepath, uploadID) => {
            imgListModel.append({
                "id": uploadID,
                "source": filepath
            });
        }
        onSignalCreateFeedbackResp: () => {
            Router.showMyFeedback(true);
        }
        onSignalAPIError: (code, type, msg) => {
            console.log("submit feedback", code, type, msg);
        }
    }

    DialogWindow {
        id: win

        // 反馈内容的提示占位，提前定义用于比较用户是否补充过信息
        property string reqPlaceholder: qsTr("[Current Status of the Product]: \n[Desired Product]: ")
        property string bugPlaceholder: qsTr("[Preconditions]: \n[Reproducibility Steps]: \n[Expected Results]: \n[Actual Results]: ")
        property string placeholder: reqType.checked ? reqPlaceholder : bugPlaceholder
        // 表单控件宽度
        property int controlWidth: width - 100 * 2

        icon: "deepin-home"
        title: qsTr("Submit Feedback")
        width: root.width * 0.8
        height: submitLayout.height + 36 + 36
        onClosing: {
            root.closed();
        }
        Component.onCompleted: {
            show();
            emailText.text = worker.getEmail();
            if (root.type == "req")
                reqType.checked = true;
            else
                bugType.checked = true;
        }

        // 提交表单
        ColumnLayout {
            id: submitLayout

            width: parent.width
            spacing: 10

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: win.title
                Layout.bottomMargin: 5
                font.pixelSize: DTK.fontManager.t6.pixelSize
                font.bold: true
            }

            // 反馈类型
            Row {
                spacing: 10

                ControlLabel {
                    text: qsTr("Type：")
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle {
                    color: Qt.rgba(0, 0, 0, 0.05)
                    height: titleText.height
                    width: win.controlWidth
                    radius: 8

                    RadioButton {
                        id: reqType

                        anchors.verticalCenter: parent.verticalCenter
                        checked: true
                        width: 150 // 控件宽度没自适应
                        text: qsTr("Suggestions")
                        font.pixelSize: DTK.fontManager.t7.pixelSize
                    }

                    RadioButton {
                        id: bugType

                        anchors.verticalCenter: parent.verticalCenter
                        width: 150 // 控件宽度没自适应
                        anchors.left: reqType.right
                        text: qsTr("Bug Report")
                        font.pixelSize: DTK.fontManager.t7.pixelSize
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
                    font.pixelSize: DTK.fontManager.t6.pixelSize
                    onTextChanged: {
                        if (length > 60)
                            remove(60, length);

                    }

                    Text {
                        x: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Please provide a brief description of your issue")
                        font.pixelSize: DTK.fontManager.t6.pixelSize
                        color: "#555"
                        opacity: 0.4
                        visible: !titleText.text && !titleText.activeFocus
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
                            color: "#555"
                            opacity: (cursorVisible || text != win.placeholder) ? 1 : 0.4
                            onTextChanged: {
                                if (length > 1000)
                                    remove(1000, length);

                            }
                            text: win.placeholder
                            Keys.onPressed: {
                                if (event.key === Qt.Key_Z && event.modifiers & Qt.ControlModifier)
                                    contentText.undo();

                            }
                        }

                    }

                    Label {
                        font: DTK.fontManager.t9
                        color: Qt.rgba(0, 0, 0, 0.6)
                        text: bugType.checked ? qsTr("Do not fill in multiple issues in one feedback") : qsTr("Do not fill in multiple requirements in one feedback")
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
                // 使用隐藏的输入框做邮箱校验，避免validator阻止用户输入，用户体验不好

                LineEdit {
                    id: emailTextValidate

                    text: emailText.text
                    visible: false

                    validator: RegExpValidator {
                        regExp: /^[a-zA-Z0-9_\.-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$/
                    }

                }

                LineEdit {
                    id: emailText

                    selectByMouse: true
                    width: win.controlWidth / 2
                    text: ""

                    Text {
                        x: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Please enter your email")
                        font.pixelSize: DTK.fontManager.t6.pixelSize
                        color: "#555"
                        opacity: 0.4
                        visible: !emailText.text && !emailText.activeFocus
                    }

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
                    width: win.controlWidth / 2
                    readOnly: true
                    text: API.sysVersion()
                }

            }

            // 上传设备信息
            Row {
                visible: bugType.checked
                spacing: 10

                ControlLabel {
                    text: qsTr("Device Info：")
                    verticalAlignment: Text.AlignVCenter
                }

                Row {
                    Switch {
                        id: sysinfoSwitch

                        height: reqType.height
                        checked: true
                    }

                    Text {
                        id: sysinfoText

                        text: qsTr("Upload <a href='home://sysinfo'>device information</a>")
                        linkColor: "blue"
                        height: reqType.height
                        verticalAlignment: Text.AlignVCenter
                        onLinkActivated: {
                            sysinfoView.visible = true;
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
                            nameFilters: [qsTr("Image files") + " (*.png *.jpg *.gif)"]
                            onAccepted: {
                                for (const f of fileDialog.fileUrls) {
                                    root.appendImage(f);
                                }
                            }
                        }

                        // 已添加图片
                        ListView {
                            id: imgList

                            width: 58 * model.count - 10
                            height: 48
                            orientation: ListView.Horizontal
                            model: root.imgListModel
                            spacing: 10

                            delegate: Rectangle {
                                color: Qt.rgba(0, 0, 0, 0.08)
                                height: imgList.height
                                width: height
                                radius: 5

                                Image {
                                    anchors.fill: parent
                                    fillMode: Image.PreserveAspectFit
                                    source: model.source
                                    onStatusChanged: {
                                        if (status == Image.Error) {
                                            API.notify(qsTr("Unable to add a screenshot."), qsTr("The image file format is not supported for uploading."));
                                            imgList.model.remove(index);
                                        }
                                    }
                                }
                                // 鼠标点击移除已添加的图片

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onEntered: {
                                        removeBtn.visible = true;
                                    }
                                    onExited: {
                                        removeBtn.visible = false;
                                    }

                                    Button {
                                        id: removeBtn

                                        anchors.fill: parent
                                        visible: false
                                        icon.name: "action_delete"
                                        icon.width: 24
                                        icon.height: 24
                                        onClicked: {
                                            imgList.model.remove(index);
                                        }
                                    }

                                }

                            }

                        }

                        Rectangle {
                            width: 48
                            height: 48

                            Image {
                                source: "qrc:///images/dotted.svg"
                                sourceSize.width: 48
                                sourceSize.height: 48
                            }
                            // 添加图片的按钮

                            Button {
                                x: 1
                                y: 1
                                width: 46
                                height: 46
                                visible: root.imgListModel.count < 3
                                icon.name: "action_add"
                                icon.width: 14
                                icon.height: 14
                                onClicked: {
                                    fileDialog.visible = true;
                                }
                            }

                        }

                    }

                    Label {
                        font: DTK.fontManager.t9
                        color: Qt.rgba(0, 0, 0, 0.6)
                        text: qsTr("Drag and drop files or click buttons to add pictures, up to three")
                    }

                }

            }

            // 底部按钮
            RowLayout {
                spacing: 10
                Layout.alignment: Qt.AlignHCenter

                DialogWindow {
                    id: cancelConfirm

                    width: 400
                    height: cancelColumn.height + 60
                    icon: "deepin-home"
                    modality: Qt.WindowModal

                    ColumnLayout {
                        id: cancelColumn

                        width: parent.width
                        spacing: 0

                        Text {
                            id: cancelText

                            Layout.alignment: Qt.AlignHCenter
                            font.pixelSize: DTK.fontManager.t5.pixelSize
                            font.weight: Font.Medium
                            text: qsTr("Are you sure you want to exit the feedback submission?")
                        }

                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            font: DTK.fontManager.t8
                            color: Qt.rgba(0, 0, 0, 0.7)
                            text: qsTr("The feedback content will not be saved.")
                        }

                        RowLayout {
                            Layout.topMargin: 30
                            Layout.alignment: Qt.AlignHCenter
                            spacing: 10

                            Button {
                                Layout.preferredWidth: 180
                                text: qsTr("Cancel")
                                font.letterSpacing: API.isZH() ? 5 : 1
                                onClicked: {
                                    cancelConfirm.close();
                                }
                            }

                            WarningButton {
                                Layout.preferredWidth: 180
                                text: qsTr("Exit")
                                font.letterSpacing: API.isZH() ? 5 : 1
                                onClicked: {
                                    cancelConfirm.close();
                                    win.close();
                                }
                            }

                        }

                    }

                }

                Button {
                    Layout.preferredWidth: 200
                    text: qsTr("Cancel")
                    font.letterSpacing: API.isZH() ? 5 : 1
                    onClicked: {
                        // 如果已填写标题或内容，弹出提示
                        if (titleText.text.length > 0 || contentText.text != win.placeholder)
                            cancelConfirm.show();
                        else
                            win.close();
                    }
                }

                RecommandButton {
                    Layout.preferredWidth: 200
                    text: qsTr("Submit")
                    font.letterSpacing: API.isZH() ? 5 : 0
                    onClicked: {
                        if (titleText.text.length == 0 && (contentText.text.length == 0 || contentText.text == win.placeholder)) {
                            API.notify(qsTr("Unable to submit feedback."), qsTr("Please provide the title and content of your feedback."));
                            return ;
                        }
                        if (titleText.text.length == 0) {
                            API.notify(qsTr("Unable to submit feedback."), qsTr("Please provide the title of your feedback."));
                            return ;
                        }
                        if (contentText.text.length == 0 || contentText.text == win.placeholder) {
                            API.notify(qsTr("Unable to submit feedback."), qsTr("Please provide the content of your feedback."));
                            return ;
                        }
                        if (emailText.text.length > 0 && !emailTextValidate.acceptableInput) {
                            API.notify(qsTr("Unable to submit feedback."), qsTr("Incorrect email address entered."));
                            return ;
                        }
                        // 保存用户邮箱，下次自动填写
                        if (emailText.text != worker.getEmail())
                            worker.setEmail(emailText.text);

                        if (!API.isLogin) {
                            API.login();
                            return ;
                        }
                        let screenshots = [];
                        for (var i = 0; i < root.imgListModel.count; i++) {
                            screenshots.push(root.imgListModel.get(i).id);
                        }
                        let type = bugType.checked ? "bug" : "req";
                        let sysinfo = (bugType.checked && sysinfoSwitch.checked) ? sysinfoView.sysinfo : "";
                        api.createFeedback(type, titleText.text, contentText.text, emailText.text, versionText.text, sysinfo, screenshots);
                    }
                }

            }

        }

        // 拖拽文件提示
        Rectangle {
            id: dropRect

            anchors.fill: parent
            visible: false
            color: Qt.rgba(0, 0, 0, 0.08)
        }

        // 拖拽文件处理
        DropArea {
            anchors.fill: parent
            onEntered: {
                dropRect.visible = true;
                drag.accept(Qt.CopyAction);
            }
            onExited: {
                dropRect.visible = false;
            }
            onDropped: {
                dropRect.visible = false;
                for (const f of drop.urls) {
                    root.appendImage(f);
                }
            }
        }

        // 设备信息
        Rectangle {
            id: sysinfoView

            property string sysinfo: ""

            visible: false
            anchors.fill: parent
            Component.onCompleted: {
                worker.getSysInfo();
            }

            Connections {
                target: worker
                function onSignalsGetSysInfoResp(sysinfo) {
                    sysinfoView.sysinfo = sysinfo;
                }
            }

            Label {
                y: 10
                text: qsTr("Device Information")
                font: DTK.fontManager.t5
                anchors.horizontalCenter: sysinfoView.horizontalCenter
            }

            ScrollView {
                width: win.width * 0.8
                height: win.height * 0.7
                anchors.centerIn: parent

                TextArea {
                    selectByMouse: true
                    readOnly: true
                    wrapMode: Text.Wrap
                    text: sysinfoView.sysinfo
                }

            }

            Button {
                width: 200
                text: qsTr("Cancel")
                anchors.bottom: sysinfoView.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: sysinfoView.horizontalCenter
                onClicked: {
                    sysinfoView.visible = false;
                }
            }

        }

        component ControlLabel: Text {
            font.pixelSize: DTK.fontManager.t6.pixelSize
            width: 100
            height: parent.height
            horizontalAlignment: Text.AlignRight
        }

    }

    imgListModel: ListModel {
    }

}
