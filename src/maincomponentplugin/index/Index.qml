// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import APIProxy 1.0
import QtQuick 2.11
import QtQuick.Window 2.15
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import org.deepin.dtk 1.0
import QtQuick.Layouts 1.15

import "../api"
import "../router"
import "../list" as DList
import "../article" as DArticle
import "../widgets" 

Item {
    id: root
    property bool showNotify
    // 首页界面
    ScrollView {
        anchors.fill: parent
        clip: true
        Column {
            id: body
            spacing: 30
            x: 20
            width: appLoader.width - 40
            height: appLoader.height
            // 轮播图
            Carousel {
                id: carousel
                width: body.width
                height: body.width*0.2
                model: ListModel {}
            }
            // 首栏
            Row {
                spacing: body.width*0.1/2
                Card1 {
                    title: qsTr("Communication")
                    subtitle: qsTr("Discuss and share here")
                    icon: "/images/bbs.svg"
                    width: body.width * 0.3
                    height: width/3
                    onClicked: {
                        API.openForum()
                    }
                }
                Card1 {
                    title: qsTr("Bug Report")
                    subtitle: qsTr("Report issues you found")
                    icon: "/images/bug.svg"
                    width: body.width * 0.3
                    height: width/3
                    onClicked: {
                        Router.showAllFeedback(false, "bug")
                    }
                }
                Card1 {
                    title: qsTr("Suggestions")
                    subtitle: qsTr("Submit your suggestions")
                    icon: "/images/demand.svg"
                    width: body.width * 0.3
                    height: width/3
                    onClicked: {
                        Router.showAllFeedback(false, "req")
                    }
                }
            }
            // 社区相关
            Column {
                spacing: 10
                width: parent.width
                Text {
                    font.pixelSize: 20
                    font.bold: true
                    text: qsTr("Resources")
                }
                Row {
                    spacing: body.width*0.1/2
                    Card2 {
                        title: qsTr("Questionaire")
                        image: "/images/q.webp"
                        width: body.width * 0.3
                        height: width/2
                        gradient: Gradient {             
                            GradientStop {  position: 0;  color: "#eff8ff" }
                            GradientStop {  position: 0.3;  color: "#f5faff" }
                            GradientStop {  position: 0.9;  color: "#e3f4ff" }
                        }
                        shadowColor: "#8dc5f0"
                        onClicked: {
                            showQuestionnaireList()
                        }
                    }
                    Card2 {
                        id: link1
                        property string url; 
                        visible: false
                        title: ""
                        image: ""
                        width: body.width * 0.3
                        height: width/2
                        gradient: Gradient {
                            GradientStop {  position: 0.2;  color: "#eef2ff" }
                            GradientStop {  position: 0.9;  color: "#e1eaff" }
                        }
                        shadowColor: "#bfbdff"
                        onClicked: {
                            Qt.openUrlExternally(url);
                        }
                    }
                    Card2 {
                        id: link2
                        property string url; 
                        visible: false
                        title: ""
                        image: ""
                        width: body.width * 0.3
                        height: width/2
                        gradient: Gradient {
                            GradientStop {  position: 0.2;  color: "#fff7ed" }
                            GradientStop {  position: 0.9;  color: "#ffefdd" }
                        }
                        shadowColor: "#FBA956"
                        onClicked: {
                            Qt.openUrlExternally(url);
                        }
                    }
                }
            }
            // 加入我们
            Column {
                spacing: 10
                width: body.width
                Text {
                    font.pixelSize: 20
                    font.bold: true
                    text: qsTr("Join US")
                }
                Row {
                    width: parent.width
                    Card3 {
                        title: qsTr("Internal Testing")
                        icon: "/images/internal.svg"
                        width: body.width * 0.5
                        height: width/4
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                dialogLoader.source = ""
                                dialogLoader.setSource("/article/Article.qml", { index: 1 })
                            }
                        }
                    }
                    Card3 {
                        title: qsTr("Community News")
                        icon: "/images/contact.svg"
                        width: body.width * 0.5
                        height: width/4
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                dialogLoader.source = ""
                                dialogLoader.setSource("/article/Article.qml", { index: 2 })
                            }
                        }
                    }
                }
            }
        }
    }
    // 对话框加载
    Loader {
        id: dialogLoader
        anchors.fill: root
    }
    // 通知弹框加载
    Loader {
        id: listLoader
        anchors.fill: root
        Connections {
            target: listLoader.item
            // 刷新页面
            function onListHide() {
                listLoader.source = ""
            }
        }
    }
    // 显示通知列表
    function showNotifyList(){
        if (listLoader.status == Loader.Null) {
            listLoader.setSource("/list/List.qml", {index: 0})
        } else {
            listLoader.item.index = 0
        }
    }
    // 显示调查问卷列表
    function showQuestionnaireList(){
        if (listLoader.status == Loader.Null) {
            listLoader.setSource("/list/List.qml", {index: 1})
        } else {
            listLoader.item.index = 1
        }
    }
    // 首页内容加载
    Component.onCompleted: {
        if (showNotify) {
            showNotifyList()
        }
        api.getClientHome()
    }


    APIProxy {
        id: api

        onSignalGetClientHomeResp: (resp) => {
            for(let item of resp.carousel){
                carousel.model.append({ img: item.img, url: item.url })
            }
            carousel.modelInited()
            if(resp.links[0]){
                link1.title = resp.links[0].title
                link1.image = resp.links[0].img
                link1.url = resp.links[0].url
                link1.visible = true
            }
            if(resp.links[1]){
                link2.title = resp.links[1].title
                link2.image = resp.links[1].img
                link2.url = resp.links[1].url
                link2.visible = true
            }
        }
    }
}