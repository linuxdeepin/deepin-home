// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import "../api"
import "../widgets"

// 显示反馈的详情，在反馈列表中点击标题进入

Item {
    id: root
    property var feedback

    ScrollView {
        anchors.fill: parent
        clip: true
        contentHeight: feedbackCard.height
        Rectangle {
            id: feedbackCard
            x: 20
            y: 10
            width: parent.width - x*2
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
                // 点赞按钮点击
                onLikeClicked: {
                    const callback = () => {
                        if(card.like){
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
                // 收藏按钮点击
                onCollectClicked: {
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
                Component.onCompleted: {
                    API.feedbackStat(feedback.public_id, (stat) => {
                        card.view_count = stat.view_count
                        card.like_count = stat.like_count
                        card.collect_count = stat.collect_count
                    })
                    API.feedbackReply(feedback.public_id, (reply) => {
                        if(reply && reply[0]) {
                            let content = reply[0].content
                            replyCard.content = content
                            replyCard.created_at = reply[0].created_at
                            replyCard.visible = true
                        }
                    })
                }
            }
            Card {
                id: replyCard
                visible: false
                anchors.top: card.bottom
                anchors.topMargin: 10
                width: parent.width
                inList: false
                title: qsTr("Official response")
                content: ""
                avatar: "http://home-dev.deepin.org/favicon.ico"
                isRelay: true
            }
        }
    }
}