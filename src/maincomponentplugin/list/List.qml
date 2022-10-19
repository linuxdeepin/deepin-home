import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.7
import org.deepin.dtk 1.0

Control {
    id: root
    property var selectedNotify
    property int index
    InWindowBlur {
        id: blur
        anchors.fill: sidebar
        radius: 20
        offscreen: false
    }
    ItemViewport {
        id: roundBlur
        anchors.fill: blur
        fixed: true
        sourceItem: blur
        radius: blur.radius
        hideSource: false
    }
    RoundRectangle {
        id: sidebar
        width: root.width
        height: root.height
        anchors.right: root.right
        color: Qt.rgba(1,1,1,0.8)
        radius: 18
        clip: true
        corners: (RoundRectangle.LeftCorner)

        Component {
            id: list
            Item {
                TabBar {
                    id: bar
                    x: 10
                    y: 10
                    width: sidebar.width-10*2
                    TabButton {
                        text: qsTr("新闻标题")
                    }
                    TabButton {
                        text: qsTr("调查问卷")
                    }
                    Component.onCompleted: {
                        bar.setCurrentIndex(root.index)
                    }
                }
                StackLayout {
                    currentIndex: bar.currentIndex
                    anchors.top: bar.bottom
                    anchors.topMargin: 10
                    anchors.bottom: parent.bottom
                    width: parent.width - 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    Item {
                        Notify {
                            anchors.fill: parent
                            onSelected: {
                                root.selectedNotify = notify
                                console.log("id", notify)
                            }
                        }
                    }
                    Item {
                        Questionnaire {
                        }
                    }
                }
            }
        }
        Component {
            id: detail
            NotifyDetail {
                title: root.selectedNotify.title
                content: root.selectedNotify.content
                start_at: root.selectedNotify.start_at
                url: root.selectedNotify.url
                onCanceled: {
                    root.selectedNotify = null
                }
            }
        }
        Loader {
            anchors.fill: parent
            sourceComponent: root.selectedNotify == null ? list : detail
        }
    }
}