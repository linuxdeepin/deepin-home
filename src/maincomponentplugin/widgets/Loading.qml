// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.deepin.dtk 1.0

Item {
    anchors.fill: parent
    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
        running: true
        width: 64
        height: 64
    }
}