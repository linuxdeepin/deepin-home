// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0

RoundRectangle {
    id: root
    property string type
    property string status
    property var statusiconList: {
        "bug-submit": "/images/status/submit.svg",
        "bug-accept": "/images/status/bug-accept.svg",
        "bug-reject": "/images/status/reject.svg",
        "bug-finish": "/images/status/finish.svg",
        "req-submit": "/images/status/submit.svg",
        "req-accept": "/images/status/req-accept.svg",
        "req-evaluate": "/images/status/evaluate.svg",
        "req-reject": "/images/status/reject.svg",
        "req-finish": "/images/status/finish.svg",
    }
    property var statusTextList: {
        "bug-submit": qsTr("Pending"),
        "bug-accept": qsTr("Confirmed"),
        "bug-reject": qsTr("Replied"),
        "bug-finish": qsTr("Resolved"),
        "req-submit": qsTr("Pending"),
        "req-evaluate": qsTr("Evaluating"),
        "req-accept": qsTr("Accepted"),
        "req-reject": qsTr("Replied"),
        "req-finish": qsTr("Completed"),
    }
    width: statusIcon.width + statusText.width + 40
    height: 36
    color: "#fff7e9"
    radius: 18
    corners: (RoundRectangle.BottomLeftCorner)
    Image {
        id: statusIcon
        source: root.statusiconList["%1-%2".arg(root.type).arg(root.status)]
        x: 16
        anchors.verticalCenter: parent.verticalCenter
    }
    Text {
        id: statusText
        anchors.left: statusIcon.right
        anchors.leftMargin: 6
        text: root.statusTextList["%1-%2".arg(root.type).arg(root.status)]
        anchors.verticalCenter: statusIcon.verticalCenter
    }
}