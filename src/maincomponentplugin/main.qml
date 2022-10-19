// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0
import QtGraphicalEffects 1.15
import "index" as DIndex
import "list" as DList

AppLoader {
    id: appLoader
    property int sidebarIndex
    Component {
        id: root
        Rectangle {
            DIndex.Index {
                id: index
                width: appLoader.width
                height: appLoader.height 
                onQuestionnaireClicked: {
                    appLoader.sidebarIndex = 1
                    sidebar.sourceComponent = list_component
                }
            }

            Component {
                id: list_component
                Item {
                    width: appLoader.width
                    height: appLoader.height 
                    DList.List {
                        id: list
                        index: appLoader.sidebarIndex
                        width: parent.width * 0.4
                        height: parent.height
                        x: parent.width
                        Behavior on x { PropertyAnimation {} }
                        Component.onCompleted: {
                            x = parent.width - width
                            console.log("completed")
                        }
                    }
                    SequentialAnimation {
                        id: destroyAnim
                        NumberAnimation { 
                            target: list
                            property:"x"
                            to: parent.width
                        }
                        ScriptAction { 
                            script: {
                                sidebar.sourceComponent = null
                            }
                        }
                    }
                    MouseArea {
                        width: parent.width * 0.6
                        height: parent.height
                        onClicked: {
                            destroyAnim.start()
                        }
                    }
                }
            }
            Loader {
                id: sidebar
            }
        }
    }
    Component.onCompleted: {
    }
}