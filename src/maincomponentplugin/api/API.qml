// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

pragma Singleton

import APIProxy 1.0
import QtQuick 2.0

Item {
    // 服务器节点
    property string node: "" 
    // 当前是否登陆
    property bool isLogin: false
    // 当前登陆的用户token
    property string token
    // 当前登陆的用户头像
    property string avatar
    // 当前登陆的用户名
    property string nickname
    // 当前的语言环境
    property string language
    // 未读消息数量
    property int msgCount: 0
    // 当前自启开关
    property bool autostart
    // 网络请求发生错误时发送该信号，在前端显示错误提示界面
    signal networkError()
    // daemon发送的托盘事件，用于控制主窗口的显示
    signal showMainWindow(bool isIconClick)

    // 预览图片，使用qml下载图片可以重用缓存
    function imagePreview(url: string) {
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = () => {
            if (xhr.readyState == XMLHttpRequest.DONE) {
                xhr.responseType="arraybuffer"
                worker.previewImage(xhr.response)
            }
        }
        xhr.open("GET", url)
        xhr.send()
    }
    // 获取系统版本
    function sysVersion() {
        return worker.sysVersion();
    }
    // 获取文件信息
    function getFileInfo(filepath) {
        if(filepath.startsWith("file://")){
            filepath=filepath.slice("file://".length)
        }
        return worker.getFileInfo(filepath)
    }
    // 发送系统通知
    function notify(title, message) {
        worker.notify(title, message)
    }
    // 登陆
    function login(notify = true) {
        if(notify) {
            API.notify(qsTr("Please log in with your Deepin ID."), qsTr("You need to log in to proceed with the subsequent operations."))
        }
        worker.login()
    }
    // 登出
    function logout() {
        worker.logout()
    }
    // 标记通知已读
    function markRead(channel, topic, uuid) {
        worker.markRead(channel, topic, uuid)
    }
    // 获取通知是否已读
    function isRead(channel, topic, uuid, callback) {
        callback(worker.isRead(channel, topic, uuid))
    }
    // 获取账户信息
    function refreshAccount() {
        isLogin = worker.isLogin()
        if(isLogin){
            let info = worker.getUserInfo()
            nickname = info.nickname
            avatar = info.avatar_url
            token = worker.getToken()
        }
    }
    // 统计未读通知
    function messageCount() {
        api.getNotify()
    }
    // 打开论坛
    function openForum() {
        worker.openForum()
    }
    // 获取自启状态
    function getAutoStart() {
        return worker.getAutoStart()
    }
    // 设置自启状态
    function setAutoStart(enable) {
        worker.setAutoStart(enable);
        autostart = enable
    }
    function isZH() {
        return language.startsWith("zh")
    }
    // 生成UUID
    function genUUID() {
        return worker.genUUID();
    }

    Component.onCompleted: {
        node = worker.getNode()
        autostart = getAutoStart()
        language = worker.getLanguage()
        refreshAccount()
        messageCount()
    }

    APIProxy {
        id: api

        onSignalGetNotifyResp: (result) => {
            const unread = result.filter(n => !worker.isRead("p", "n", n.uuid)).length
            msgCount = unread
        }
    }

    Connections {
        target: worker
        function onUserInfoChanged() {
            console.log("user info changed")
            refreshAccount()
        }
        function onMessageChanged() {
            console.log("message change")
            messageCount()
        }
        function onShowMainWindow(isIconClick) {
            console.log("showMainWindow")
            showMainWindow(isIconClick)
        }
        function onNetworkError() {
            console.log("onNetworkError")
            networkError()
        }
    }
}