// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

pragma Singleton
import QtQuick 2.0
import QtQuick.LocalStorage 2.0

Item {
    function get(url, callback) {
        url = worker.getNode() + url
        console.log("send get request", url)
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && xhr.status == 200) {
                callback(JSON.parse(xhr.responseText))
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
        getLanguage(lang=>{
            get("/api/v1/public/channel/p/topic/n/messages?language="+lang, callback)
        })
    }
    // 获取调查问卷列表
    function getQuestionnaire(callback) {
        getLanguage(lang=>{
            get("/api/v1/public/channel/p/topic/q/messages?language="+lang, callback)
        })
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
    // 标记通知已读
    function markRead(channel, topic, uuid) {
        worker.markRead(channel, topic, uuid)
    }
    // 获取通知是否已读
    function isRead(channel, topic, uuid, callback) {
        callback(worker.isRead(channel, topic, uuid))
    }
    Component.onCompleted: {
        getLanguage(lang => {})
        console.log(worker.getNode());
    }
}