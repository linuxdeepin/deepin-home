// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later

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

    required property var feedback
    property var reply: null

    Component.onCompleted: {
        if (!feedback)
            api.getFeedback(public_id);
        else
            loader.sourceComponent = main;
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

    APIProxy {
        id: api

        onSignalGetFeedbackResp: (feedback) => {
            api.viewFeedback(feedback.public_id);
            Router.showFeedbackDetail(feedback);
        }
        onSignalGetFeedbackReplyResp: (reply) => {
            root.reply = reply;
        }
    }

    Loader {
        id: loader

        sourceComponent: loading
    }

    Component {
        id: main

        ScrollView {
            width: root.width
            height: root.height
            clip: true
            contentHeight: feedbackCard.height

            Rectangle {
                id: feedbackCard

                x: 20
                y: 10
                width: parent.width - x * 2
                height: (card.height + 20) + (replyCard.visible ? (replyCard.height + 30) : 0)

                Card {
                    id: card

                    width: parent.width
                    inList: false
                    public_id: feedback.public_id
                    title: feedback.title
                    content: feedback.content
                    type: feedback.type
                    status: feedback.status
                    created_at: feedback.created_at
                    like: feedback.like
                    collect: feedback.collect
                    screenshots: feedback.screenshots
                    avatar: feedback.avatar
                    system_version: feedback.system_version
                    view_count: feedback.view_count
                    like_count: feedback.like_count
                    collect_count: feedback.collect_count
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
                        console.log("open screenshot", img);
                        API.imagePreview(img);
                    }
                    Component.onCompleted: {
                        api.getFeedbackReply(feedback.public_id);
                    }
                }

                Card {
                    id: replyCard

                    visible: reply
                    anchors.top: card.bottom
                    anchors.topMargin: 10
                    width: parent.width
                    inList: false
                    title: qsTr("Official response")
                    content: reply.content
                    created_at: reply.created_at
                    avatar: "https://home.deepin.org/favicon.ico"
                    isReply: true
                    onLinkClicked: (link) => {
                        if (link.startsWith("#")) {
                            const public_id = link.slice(1);
                            api.getFeedback(public_id);
                            return ;
                        }
                        Qt.openUrlExternally(link);
                    }
                }

            }

        }

    }

    Component {
        id: loading

        Rectangle {
            id: loading

            width: root.width
            height: root.height

            Loading {
                anchors.centerIn: parent
            }

        }

    }

}
