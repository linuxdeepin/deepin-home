// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import "../api"
import "../router"
import "../widgets"

// 反馈列表组建
// 在反馈广场、我的反馈、我的收藏和我的关注中均有使用

Item {
    id: root
    // 反馈和用户的关联关系，值有 like(点赞) collect(收藏) create(创建) 为空时，则获取所有反馈
    property string relation: ""
    // 分页显示
    property int offset: 0
    property int limit: 5
    // 是否显示反馈类型过滤
    property bool typeFilter: false
    property string type: ""
    // 是否显示“加载更多”按钮
    property bool hasMore: true

    ListModel {
        id: feedbackList
    }
    // 获取反馈列表
    function getList(clear) {
        // 在切换分类过滤时，重置列表
        if(clear) {
            root.offset = 0
            root.hasMore = true
            feedbackList.clear()
        }
        API.getFeedback({"offset": root.offset, "limit": root.limit, "relation": root.relation, "type": root.type},(resp)=>{
            root.offset += root.limit
            // 返回的条数小于要求的条数，则不再显示“加载更多”按钮
            if(resp.length != root.limit) {
                root.hasMore = false
            }
            for(const feedback of resp){
                feedbackList.append({
                    feedback: feedback
                })
            }
        })
    }
    Component.onCompleted: {
        getList(true)
    }

    Rectangle {
        anchors.fill: parent
        clip: true
        ScrollView {
            // 给分类下拉框留空
            y: root.typeFilter ? 56 : 10
            width: parent.width
            height: parent.height - y
            ListView {
                id: listView
                spacing: 10
                model: feedbackList
                delegate: Item {
                    x: 20
                    y: 10
                    width: listView.width - x*2
                    height: card.height + (moreBtn.visible ? moreBtn.height : 0)
                    Card {
                        id: card
                        width: parent.width
                        public_id: feedback.public_id
                        title: feedback.title
                        content: feedback.content
                        type: feedback.type
                        status: feedback.status
                        created_at: feedback.created_at
                        like: feedback.like
                        collect: feedback.collect

                        // 点击标题时，进入详情
                        onTitleClicked: {
                            Router.showFeedbackDetail(feedback)
                        }
                    }
                    // 在底部显示“加载更多”按钮
                    Rectangle {
                        id: moreBtn
                        anchors.top: card.bottom
                        visible: index === feedbackList.count - 1 && root.hasMore
                        width: parent.width
                        height: 56
                        Button {
                            anchors.centerIn: parent
                            text: qsTr("Load More")
                            onClicked: {
                                getList(false)
                            }
                        }
                    }
                }
            }
        }
        // 分类过滤下拉框
        Rectangle {
            id: headerRect
            width: parent.width
            height: 56
            visible: root.typeFilter
            ComboBox {
                id: selectBox
                width: 150
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                textRole: "text"
                model: ListModel {
                    id: selectOptions
                    ListElement { text: qsTr("All"); value: "" }
                    ListElement { text: qsTr("Bug"); value: "bug" }
                    ListElement { text: qsTr("Suggestions"); value: "req" }
                }
                onActivated: {
                    root.type = selectOptions.get(currentIndex).value
                    root.getList(true)
                }
            }
        }

        // 没有反馈数据时，显示提示
        NotFound {
            visible: feedbackList.count === 0
            anchors.centerIn: parent
            title: ""
        }
    }
}