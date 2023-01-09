// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

pragma Singleton
import QtQuick 2.0

Item {
    property bool isLogin: false
    property string avatar
    property string nickname
    property int msgCount: 0
    property bool autostart
    signal networkError()
    signal showMainWindow()

    function get(url, callback) {
        url = worker.getNode() + url
        console.log("send get request", url)
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4) {
                if(xhr.status == 200) {
                    callback(JSON.parse(xhr.responseText))
                } else {
                    console.log("network error", xhr.status)
                    networkError()
                }
            }
        }
        xhr.open("GET", url , true)
        xhr.send()
    }
    // 获取服务器判定语言（归类）
    function getLanguage(callback) {
        callback(worker.getLanguage())
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
        getLanguage(lang=>{
            get("/api/v1/public/setting/internal-test_"+lang, callback)
        })
    }
    // 获取社区动态内容
    function getAboutUs(callback){
        getLanguage(lang=>{
            get("/api/v1/public/setting/aboutus_"+lang, callback)
        })
    }
    function getClientHome(callback) {
        getLanguage(lang=>{
            get("/api/v1/public/setting/client-home_"+lang, (resp)=>{
                callback(JSON.parse(resp.value))
            })
        })
    }
    function login() {
        worker.login()
    }
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
        }
    }
    // 统计未读通知
    function messageCount() {
        getNotify(list => {
            const unread = list.filter(n => !worker.isRead("p", "n", n.uuid)).length
            msgCount = unread
        })
    }
    function openForum() {
        worker.openForum()
    }
    function getAutoStart() {
        return worker.getAutoStart()
    }
    function setAutoStart(enable) {
        worker.setAutoStart(enable);
        autostart = enable
    }

    Component.onCompleted: {
        refreshAccount()
        messageCount()
        autostart = getAutoStart()
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
        function onShowMainWindow() {
            console.log("showMainWindow")
            showMainWindow()
        }
    }
}