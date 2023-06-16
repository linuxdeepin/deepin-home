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
    property var statusBgColor: {
        "bug-submit": "#FFF7E9",
        "bug-accept": "#D6F7FF",
        "bug-reject": "#F5FFE1",
        "bug-finish": "#E1FFF1",
        "req-submit": "#FFF7E9",
        "req-evaluate": "#E0E5FF",
        "req-accept": "#FFF7E9",
        "req-reject": "#F5FFE1",
        "req-finish": "#E1FAFF",
    }
    property var statusTextColor: {
        "bug-submit": "#B78C45",
        "bug-accept": "#0261BF",
        "bug-reject": "#17794B",
        "bug-finish": "#17794B",
        "req-submit": "#B78C45",
        "req-evaluate": "#0050A9",
        "req-accept": "#B78C45",
        "req-reject": "#17794B",
        "req-finish": "#17794B",
    }
    width: statusIcon.width + statusText.width + 40
    height: 36
    color: root.statusBgColor["%1-%2".arg(root.type).arg(root.status)]
    radius: 18
    corners: (RoundRectangle.BottomLeftCorner | RoundRectangle.TopRightCorner)
    Image {
        id: statusIcon
        source: root.statusiconList["%1-%2".arg(root.type).arg(root.status)]
        x: 16
        anchors.verticalCenter: parent.verticalCenter
    }
    Text {
        color: root.statusTextColor["%1-%2".arg(root.type).arg(root.status)]
        id: statusText
        anchors.left: statusIcon.right
        anchors.leftMargin: 6
        text: root.statusTextList["%1-%2".arg(root.type).arg(root.status)]
        anchors.verticalCenter: statusIcon.verticalCenter
    }
}