import QtQuick 2.11
import QtQuick.Controls 2.4
import "../api"

Control {
    id: root
    signal selected(var notify)
    ListView {
        id: list_listview
        anchors.fill: parent
        model: ListModel {
            ListElement {
                uuid: "item.uuid"
                title: "item.title"
                summary: "item.summary"
                start_at: "item.start_at"
                content: "item.content"
                url: ""
            }
        }
        API {
            id: api
        }
        Component.onCompleted: {
            api.getNotify((result) => {
                for(let item of result){
                    list_listview.model.append({
                        "uuid": item.uuid,
                        "title": item.title,
                        "summary": item.summary,
                        "start_at": item.start_at,
                        "content": item.content,
                        "url": item.url,
                    })
                    console.log(item.summary)
                }
            })
        }
        delegate: Rectangle {
            width: sidebar.width - 20
            height: 66
            radius: 8
            color: index%2==0 ? Qt.rgba(0,0,0,0.05) : 'transparent';
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    parent.color=index%2==0 ? Qt.rgba(0,0,0,0.2) : Qt.rgba(0,0,0,0.1)
                }
                onExited: {
                    parent.color=index%2==0 ? Qt.rgba(0,0,0,0.05) : 'transparent'
                }
                onPressed: {
                    parent.color=Qt.rgba(0,0,0,0.3)
                }
                onReleased: {
                    parent.color=index%2==0 ? Qt.rgba(0,0,0,0.05) : 'transparent'
                }
                onClicked: {
                    console.log("select notfiy", index, uuid)
                    root.selected({title, content, start_at, url})
                }
            }
            Rectangle {
                color: "transparent"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 12
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 12
                Text {
                    id: title_text
                    x: 20
                    text: title
                    font.pixelSize: 14
                    font.bold: true
                }
                Rectangle {
                    width: 8
                    height: 8
                    radius: 4
                    color: "red"
                    anchors.right: title_text.left
                    anchors.rightMargin: 6
                    anchors.verticalCenter: title_text.verticalCenter
                }
                Text {
                    x: 20
                    text: summary
                    anchors.top: title_text.bottom
                    color: Qt.rgba(0,0,0,0.7);
                    width: parent.width
                    elide: Text.ElideRight
                }
                Text {
                    text: start_at
                    anchors.right: parent.right
                    color: Qt.rgba(0,0,0,0.6);
                }
            }
        }
    }
}