// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

pragma Singleton
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
    // 反馈列表更新
    signal signalFeedbackListUpdate(string reqID, var feedbacks)
    // 发送http请求
    function request(method, rawUrl, body, callback) {
        const url = node + rawUrl
        console.log("send %1 request".arg(method), url)
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = () => {
            if (xhr.readyState == XMLHttpRequest.DONE) {
                switch(xhr.status){
                    case 200:
                        if(xhr.responseText.length > 0) {
                            callback(JSON.parse(xhr.responseText))
                        } else {
                            callback(null)
                        }
                        break
                    case 401:
                        login()
                        break
                    case 429:
                        notify(qsTr("You've been making too many requests. Please try again later."))
                        break
                    default:
                        console.log("network error", xhr.status)
                        networkError()
                }
            }
        }
        xhr.open(method, url)
        // 调用用户接口时，在 header 添加用户 token
        if(rawUrl.startsWith("/api/v1/user")) {
            xhr.setRequestHeader("Authorization", "Bearer " + token)
        }
        if(body){
            xhr.send(JSON.stringify(body))
        } else {
            xhr.send()
        }
    }
    // 发送 get 请求
    function get(url, callback) {
        return request("GET", url, null, callback)
    }
    // 发送 put 请求
    function put(url, body, callback) {
        return request("PUT", url, body, callback)
    }
    // 发送 post 请求
    function post(url, body, callback) {
        return request("POST", url, body, callback)
    }
    // 发送 delete 请求， 因为和 delete 关键字冲突，最后加了下划线
    function delete_(url, callback) {
        return request("DELETE", url, null, callback)
    }
    // 获取通知列表
    function getNotify(callback) {
        // p: public channel
        let data = worker.getMessages("p", "n")
        callback(JSON.parse(data))
    }
    // 获取调查问卷列表
    function getQuestionnaire(callback) {
        // p: public channel
        let data = worker.getMessages("p", "q")
        callback(JSON.parse(data))
    }
    // 获取内测渠道内容
    function getInternalTest(callback){
        get("/api/v1/public/setting/internal-test_"+language, callback)
    }
    // 获取社区动态内容
    function getAboutUs(callback){
        get("/api/v1/public/setting/aboutus_"+language, callback)
    }
    // 获取首页配置
    function getClientHome(callback) {
        get("/api/v1/public/setting/client-home_"+language, (resp)=>{
            callback(JSON.parse(resp.value))
        })
    }

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
    // 上传文件
    function upload(filepath, callback) {
        const info = getFileInfo(filepath)
        post("/api/v1/user/upload/pre", {name: info.filename, size: info.size}, (preInfo) => {
            let err = worker.uploadFile(preInfo.url, info.filepath, preInfo.form_data)
            if(err){
                networkError()
                return
            }
            callback(preInfo.id)
        })
    }
    // 创建反馈
    function createFeedback(feedback, callback) {
        post("/api/v1/user/feedback", feedback, callback)
    }
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
        getNotify(list => {
            const unread = list.filter(n => !worker.isRead("p", "n", n.uuid)).length
            msgCount = unread
        })
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
    }
}