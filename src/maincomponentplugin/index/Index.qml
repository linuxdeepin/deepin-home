// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import org.deepin.dtk 1.0

import "../api"
import "../list" as DList
import "../article" as DArticle


Item {
    id: root
    property int listIndex
    property int articleIndex

    signal questionnaireClicked()
    signal questionnaireCancel()

    onListIndexChanged: {
        if(listIndex >= 0){
            popup.sourceComponent = list_component
        } else {
            popup.sourceComponent = null
        }
    }
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
                    title: "互动交流"
                    subtitle: "社区用户交流、分析"
                    icon: "/images/bbs.svg"
                    width: body.width * 0.3
                    height: width/3
                    onClicked: {
                        API.openForum()
                    }
                }
                Card1 {
                    title: "Bug反馈"
                    subtitle: "日常使用问题反馈"
                    icon: "/images/bug.svg"
                    disabled: true
                    width: body.width * 0.3
                    height: width/3
                }
                Card1 {
                    title: "需求反馈"
                    subtitle: "需求反馈"
                    icon: "/images/demand.svg"
                    disabled: true
                    width: body.width * 0.3
                    height: width/3
                }
            }
            // 社区相关
            Column {
                spacing: 10
                width: parent.width
                Text {
                    font.pointSize: 20
                    font.bold: true
                    text: qsTr("社区相关")
                }
                Row {
                    spacing: body.width*0.1/2
                    Card2 {
                        title: "问卷调查"
                        image: "https://storage.deepin.org/forum/2022061003111206_1654845072.jpg"
                        width: body.width * 0.3
                        height: width/2
                        gradient: Gradient {             
                            GradientStop {  position: 0;  color: "#eff8ff" }
                            GradientStop {  position: 0.3;  color: "#f5faff" }
                            GradientStop {  position: 0.9;  color: "#e3f4ff" }
                        }
                        shadowColor: "#8dc5f0"
                        onClicked: {
                            root.questionnaireClicked()
                            // root.listIndex = 1
                        }
                    }
                    Card2 {
                        title: "WIKI"
                        image: "https://storage.deepin.org/forum/2022090101422509_1662010945.jpg"
                        width: body.width * 0.3
                        height: width/2
                        gradient: Gradient {
                            GradientStop {  position: 0.2;  color: "#eef2ff" }
                            GradientStop {  position: 0.9;  color: "#e1eaff" }
                        }
                        shadowColor: "#bfbdff"
                        onClicked: {
                            Qt.openUrlExternally("https://wiki.deepin.org");
                        }
                    }
                    Card2 {
                        title: "GitHub"
                        image: "https://storage.deepin.org/forum/2022092001093009_1663650570.jpg"
                        width: body.width * 0.3
                        height: width/2
                        gradient: Gradient {
                            GradientStop {  position: 0.2;  color: "#fff7ed" }
                            GradientStop {  position: 0.9;  color: "#ffefdd" }
                        }
                        shadowColor: "#FBA956"
                        onClicked: {
                            Qt.openUrlExternally("https://github.com/linuxdeepin");
                        }
                    }
                }
            }
            // 加入我们
            Column {
                spacing: 10
                width: body.width
                Text {
                    font.pointSize: 18
                    font.bold: true
                    text: qsTr("加入我们")
                }
                Row {
                    width: parent.width
                    Card3 {
                        title: "内测交流"
                        icon: "/images/internal.svg"
                        width: body.width * 0.5
                        height: width/4
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                root.articleIndex = 1
                                popup.sourceComponent = article_component
                            }
                        }
                    }
                    Card3 {
                        title: "社区动态"
                        icon: "/images/contact.svg"
                        width: body.width * 0.5
                        height: width/4
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                root.articleIndex = 2
                                popup.sourceComponent = article_component
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        API.getClientHome(resp=>{
            for(let item of resp.carousel){
                carousel.model.append({img: item.img, url: item.url})
            }
        })
    }

    Component {
        id: list_component
        Item {
            width: appLoader.width
            height: appLoader.height 
            Rectangle {
                anchors.fill: parent
                color: Qt.rgba(0,0,0,0.2)
                
                MouseArea {
                    width: parent.width*0.6
                    height: parent.height
                    onClicked: {
                        destroyAnim.start()
                    }
                }
            }
            DList.List {
                id: list
                index: root.listIndex
                width: parent.width * 0.4
                height: parent.height
                x: parent.width
                Behavior on x { PropertyAnimation {} }
                Component.onCompleted: {
                    x = parent.width - width
                    console.log("completed")
                }
            }
            SequentialAnimation {
                id: destroyAnim
                NumberAnimation { 
                    target: list
                    property:"x"
                    to: parent.width
                }
                ScriptAction { 
                    script: {
                        root.questionnaireCancel()
                    }
                }
            }
        }
    }

    Component {
        id: article_component
        Item {
            DArticle.Article {
                width: root.width
                height: root.height
                index: root.articleIndex
                onCanceled: {
                    popup.sourceComponent = null
                }
            }
        }
    }

    Loader {
        id: popup
    }
}