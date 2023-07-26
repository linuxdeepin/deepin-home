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
    // 反馈和用户的关联关系，值有 like(点赞) collect(收藏) 为空时，则获取所有反馈
    property string relation: ""
    // 分页显示
    property int offset: 0
    property int limit: 5
    // 是否显示反馈类型过滤
    property bool typeFilter: false
    property string type: ""
    // 是否显示“加载更多”按钮
    property bool hasMore: true
    property bool loadMore: false

    ListModel {
        id: feedbackList
    }
    function goDetail(feedback) {
        API.publicViewFeedback(feedback.public_id)
        if(API.isLogin) {
            API.userViewFeedback(feedback.public_id)
        }
        Router.showFeedbackDetail(feedback)
    }
    // 获取反馈列表
    function getList(clear) {
        // 在切换分类过滤时，重置列表
        if(clear) {
            root.offset = 0
            root.hasMore = true
            feedbackList.clear()
        }
        const opt = {
            "offset": root.offset,
            "limit": root.limit,
            "relation": root.relation,
            "type": root.type,
        }
        const callback = (resp)=>{
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
            root.loadMore = false
        }
        switch(root.relation){
            case "create":
                API.getMyFeedback(opt, callback)
                break
            case "collect":
            case "like":
                // 获取我的收藏和我的关注
                API.getRelation(opt, callback)
                break
            case "":
                // 获取反馈广场
                API.getFeedback(opt, callback)
                break
        }
    }
    Component.onCompleted: {
        getList(true)
    }

    // 分类过滤下拉框
    Rectangle {
        id: headerRect
        width: parent.width
        height: selectBox.height + 20
        visible: root.typeFilter
        ComboBox {
            id: selectBox
            width: 150
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            textRole: "text"
            model: ListModel {
                id: selectOptions
                ListElement { text: qsTr("All"); value: "" }
                ListElement { text: qsTr("Bug"); value: "bug" }
                ListElement { text: qsTr("Suggestions"); value: "req" }
            }
            onActivated: {
                if(root.relation=="create"){
                    Router.showMyFeedback(true, selectOptions.get(currentIndex).value, true)
                    return
                }
                Router.showAllFeedback(true, selectOptions.get(currentIndex).value, true)
            }
            Component.onCompleted:{
                if(root.type === "bug") {
                    currentIndex = 1
                } else if(root.type === "req") {
                    currentIndex = 2
                } else {
                    currentIndex = 0
                }
            }
        }
    }
    
    ScrollView {
        // 给分类下拉框留空
        y: root.typeFilter ? headerRect.height - 10 : 10
        width: parent.width
        height: parent.height - y - 10
        clip: true
        ScrollBar.vertical.onPositionChanged: () => {
            const position = ScrollBar.vertical.position + ScrollBar.vertical.size
            if(position > 0.98 && !root.loadMore && root.hasMore) {
                root.loadMore = true
                timer.start()
            }
        }
        ListView {
            id: listView
            spacing: 10
            model: feedbackList
            delegate: Item {
                x: 20
                width: listView.width - x*2
                height: card.height + card.y + ( index == feedbackList.count-1 ? 20 : 0)
                Card {
                    id: card
                    y: index === 0 ? 10 : 0;
                    width: parent.width
                    public_id: feedback.public_id
                    title: feedback.title
                    content: feedback.content
                    type: feedback.type
                    status: feedback.status
                    created_at: feedback.created_at
                    like: feedback.like || false
                    collect: feedback.collect || false
                    screenshots: feedback.screenshots
                    system_version: feedback.system_version || ""
                    avatar: feedback.avatar
                    view_count: feedback.view_count
                    like_count: feedback.like_count
                    collect_count: feedback.collect_count
                    // 点击标题时，进入详情
                    onTitleClicked: {
                        const value = feedback
                        value.like = card.like
                        value.collect = card.collect
                        root.goDetail(value)
                    }
                    // 点赞按钮点击
                    onLikeClicked: {
                        if(!API.isLogin){
                            API.login()
                            return
                        }
                        const callback = () => {
                            if (card.like) {
                                card.like_count--
                            } else {
                                card.like_count++
                            }
                            card.like = !card.like
                        }
                        if (card.like) {
                            API.cancelLikeFeedback(feedback.public_id, callback)
                        } else {
                            API.likeFeedback(feedback.public_id, callback)
                        }
                    }
                    onImageClicked: (img) => {
                        console.log("preview screenshot", img)
                        API.imagePreview(img)
                    }
                    // 收藏按钮点击
                    onCollectClicked: {
                        if(!API.isLogin){
                            API.login()
                            return
                        }
                        // 点击可以收藏和取消收藏
                        const callback = () => {
                            if (card.collect) {
                                card.collect_count--
                            } else {
                                card.collect_count++
                            }
                            card.collect = !card.collect
                        }
                        if (card.collect) {
                            API.cancelCollectFeedback(feedback.public_id, callback)
                        } else {
                            API.collectFeedback(feedback.public_id, callback)
                        }
                    }
                }
            }
        }
    }
    Timer {
        id: timer
        interval: 1000
        repeat: false
        onTriggered: {
            getList(false)
        }
    }
    BusyIndicator {
        id: indicator
        visible: root.loadMore
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: root.bottom
        anchors.bottomMargin: 2
        running: true
        width: 16
        height: 16
    }
    // 没有反馈数据时，显示提示
    NotFound {
        visible: feedbackList.count === 0
        anchors.centerIn: parent
        title: ""
    }
    // 只在需求广场显示的“提交反馈”的按钮，点击后弹出提交反馈的对话框
    FloatingButton {
        id: submitButton
        visible: submitLoader.source=="" && Router.routeCurrent.path === Router.routeAllFeedback.path
        width: 48
        height: 48
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 27
        anchors.horizontalCenter: parent.horizontalCenter
        icon.name: "action_add"
        icon.width: 18
        icon.height: 18
        onClicked: {
            submitLoader.setSource("Submit.qml", {type: root.type})
        }
    }
    Loader {
        id: submitLoader
        anchors.fill: root
        Connections {
            target: submitLoader.item
            function onClosed() {
                submitLoader.source=""
            }
        }
    }
}