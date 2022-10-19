import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0
import "../api"

ListView {
    id: root
    anchors.fill: parent
    model: ListModel {}
    API {
        id: api
    }
    Component.onCompleted: {
        api.getQuestionnaire((result)=>{
            for(let item of result){
                root.model.append({"title": item.title, "summary": item.summary, "start_at": item.start_at})
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
                parent.color=index%2==0 ? Qt.rgba(0,0,0,0.2) : 'transparent'
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
                text: summary
                anchors.top: title_text.bottom
                anchors.left: title_text.left
                color: Qt.rgba(0,0,0,0.7);
                width: parent.width
                elide: Text.ElideRight
            }
            Text {
                text: start_at
                anchors.right: parent.right
                color: Qt.rgba(0,0,0,0.6);
            }
            RecommandButton {
                text: "填写"
                width: 60
                height: 26
                anchors.right: parent.right
                anchors.top: title_text.bottom
            }
        }
    }
}