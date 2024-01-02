// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later
// 反馈列表组件
// 在反馈广场、我的反馈、我的收藏和我的关注中均有使用

import "../api"
import "../router"
import "../widgets"
import APIProxy 1.0
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0

Item {
    id: root

    // 反馈和用户的关联关系，值有 like(点赞) collect(收藏) 为空时，则获取所有反馈
    property string relation: ""
    // 是否正在初始化加载
    property bool initing: true
    // 分页显示
    property int offset: 0
    property int limit: 5
    // 是否显示反馈类型过滤
    property bool typeFilter: false
    // 当前反馈类型
    property string type: ""
    // 是否显示“加载更多”按钮
    property bool hasMore: true
    // 是否正在“加载更多”
    property bool loadMore: false
    property var filterParam: {
        "type": root.type,
        "order": '',
        "keyword": ''
    }

    function goDetail(feedback) {
        api.viewFeedback(feedback.public_id);
        Router.showFeedbackDetail(feedback);
    }

    // 获取反馈列表
    function getList(clear) {
        // 在切换分类过滤时，重置列表
        if (clear) {
            root.initing = true;
            root.offset = 0;
            root.hasMore = true;
            feedbackList.clear();
        }
        switch (root.relation) {
        case "create":
            // 我的反馈只保留类型筛选
            filter.controls = ["type"];
            // 获取我的反馈
            api.getUserFeedback(root.offset, root.limit, root.type);
            break;
        case "like":
            // 获取我的关注
            api.getLikeFeedback(root.offset, root.limit);
            break;
        case "collect":
            // 获取我的收藏
            api.getCollectFeedback(root.offset, root.limit);
            break;
        case "":
            // 获取反馈广场
            api.allFeedback(root.offset, root.limit, filterParam);
            break;
        }
    }

    Component.onCompleted: {
        getList(true);
    }

    ListModel {
        id: feedbackList
    }

    Connections {
        function onUserInfoChanged() {
            if (root.relation != "") {
                Router.showIndex()
            }
        }

        target: worker
    }

    APIProxy {
        id: api

        // 列表加载完成后进行处理
        function listResult(feedbacks) {
            root.offset += root.limit;
            // 返回的条数小于要求的条数，则不再显示“加载更多”按钮
            if (feedbacks.length != root.limit)
                root.hasMore = false;

            const keyword = root.filterParam["keyword"];
            for (const feedback of feedbacks) {
                feedbackList.append({
                    "feedback": feedback
                });
            }
            root.loadMore = false;
            root.initing = false;
        }

        onSignalAllFeedbackResp: (feedbacks) => {
            console.log("onSignalGetFeedbackResp");
            listResult(feedbacks);
        }
        onSignalGetLikeFeedbackResp: (feedbacks) => {
            console.log("onSignalGetLikeFeedbackResp");
            listResult(feedbacks);
        }
        onSignalGetCollectFeedbackResp: (feedbacks) => {
            console.log("onSignalGetCollectFeedbackResp");
            listResult(feedbacks);
        }
        onSignalGetUserFeedbackResp: (feedbacks) => {
            console.log("onSignalGetCollectFeedbackResp");
            listResult(feedbacks);
        }
        onSignalAPIError: {
            root.loadMore = false;
            root.initing = false;
        }
    }

    // 分类过滤下拉框
    Rectangle {
        id: headerRect

        width: parent.width
        height: 64
        visible: root.typeFilter

        Filter {
            id: filter

            x: 20
            width: parent.width - x * 2
            height: parent.height
            value: filterParam
            onValueChange: (val) => {
                filterParam = val;
                root.type = filterParam['type'];
                getList(true);
            }
        }

    }

    // 反馈列表
    ScrollView {
        // 给分类下拉框留空
        y: root.typeFilter ? headerRect.height - 10 : 10
        width: parent.width
        height: parent.height - y
        clip: true
        ScrollBar.vertical.onPositionChanged: () => {
            const position = ScrollBar.vertical.position + ScrollBar.vertical.size;
            if (position > 0.9 && !root.loadMore && root.hasMore) {
                root.loadMore = true;
                timer.restart();
            }
        }

        ColumnLayout {
            x: 20
            y: 10
            width: parent.width - x * 2
            spacing: 10

            Repeater {
                id: listView

                model: feedbackList

                delegate: Item {
                    required property int index
                    required property var feedback

                    Layout.fillWidth: true
                    height: card.height

                    Card {
                        id: card

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
                            const value = feedback;
                            value.like = card.like;
                            value.collect = card.collect;
                            value.like_count = card.like_count;
                            value.collect_count = card.collect_count;
                            root.goDetail(value);
                        }
                        // 点赞按钮点击
                        onLikeClicked: {
                            if (!API.isLogin) {
                                API.login();
                                return ;
                            }
                            if (card.like) {
                                card.like = false;
                                card.like_count--;
                                debounceTimer.callback = () => {
                                    api.cancelLikeFeedback(feedback.public_id);
                                };
                            } else {
                                card.like = true;
                                card.like_count++;
                                debounceTimer.callback = () => {
                                    api.likeFeedback(feedback.public_id);
                                };
                            }
                            debounceTimer.restart();
                        }
                        // 收藏按钮点击
                        onCollectClicked: {
                            if (!API.isLogin) {
                                API.login();
                                return ;
                            }
                            if (card.collect) {
                                card.collect = false;
                                card.collect_count--;
                                debounceTimer.callback = () => {
                                    api.cancelCollectFeedback(feedback.public_id);
                                };
                            } else {
                                card.collect = true;
                                card.collect_count++;
                                debounceTimer.callback = () => {
                                    api.collectFeedback(feedback.public_id);
                                };
                            }
                            debounceTimer.restart();
                        }
                        onImageClicked: (img) => {
                            console.log("preview screenshot", img);
                            API.imagePreview(img);
                        }

                        // 节流定时器
                        Timer {
                            id: debounceTimer

                            property var callback: () => {
                            }

                            interval: 300
                            repeat: false
                            onTriggered: {
                                callback();
                            }
                        }

                    }

                }

            }

            // 在尾部保留20像素间距，避免最后一个反馈显示不完整
            Rectangle {
                visible: hasMore
                height: 20
            }

            // 没有更多反馈了
            Rectangle {
                visible: !hasMore && feedbackList.count > root.limit
                Layout.fillWidth: true
                height: 35

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Qt.rgba(0, 0, 0, 0.6)
                    text: qsTr("No more items")
                }

            }

        }

    }

    // 加载更多反馈
    Timer {
        id: timer

        interval: 1000
        repeat: false
        onTriggered: {
            getList(false);
        }
    }

    // 加载更多动画
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

    // 没有反馈数据时，提示空列表
    NotFound {
        visible: feedbackList.count === 0
        anchors.centerIn: parent
        title: ""
    }

    // 只在需求广场显示的“提交反馈”的按钮，点击后弹出提交反馈的对话框
    FloatingButton {
        id: submitButton

        visible: submitLoader.source == "" && Router.routeCurrent.path === Router.routeAllFeedback.path
        width: 48
        height: 48
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 27
        anchors.horizontalCenter: parent.horizontalCenter
        icon.name: "action_add"
        icon.width: 18
        icon.height: 18
        onClicked: {
            submitLoader.setSource("Submit.qml", {
                "type": root.type
            });
        }
    }

    // 提交反馈
    Loader {
        id: submitLoader

        anchors.fill: root

        Connections {
            function onClosed() {
                submitLoader.source = "";
            }

            target: submitLoader.item
        }

    }

    // 初始化加载
    Rectangle {
        visible: initing
        anchors.fill: root

        Loading {
            anchors.centerIn: parent
        }

    }

}
