// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4


ScrollView {
    id: root
    signal notifyClicked()
    signal questionnaireClicked()
    Column {
        id: body
        spacing: 30
        x: 20
        width: appLoader.width - 40
        height: appLoader.height
        // 轮播图
        Carousel {
            width: body.width
            height: body.width*0.2
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
                    Qt.openUrlExternally("https://bbs.deepin.org");
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
                        console.log("notifyClicked")
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
                }
                Card3 {
                    title: "社区动态"
                    icon: "/images/contact.svg"
                    width: body.width * 0.5
                    height: width/4
                }
            }
        }
    }
}