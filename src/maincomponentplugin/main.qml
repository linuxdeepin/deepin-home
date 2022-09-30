// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import "index"

AppLoader {
    id: appLoader
    Component {
        id: index
        Index {
            width: appLoader.width
            height: appLoader.height
        }
    }
}