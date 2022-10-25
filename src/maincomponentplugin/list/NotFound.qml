import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0

// 无数据时显示
Item {
    property string title
    property string subtitle
    Image {
        sourceSize.width: 130
        sourceSize.height: 130
        source: "/images/404.svg"
        anchors.bottom: title_text.top
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Text {
        id: title_text
        anchors.centerIn: parent
        font.pixelSize: 16
        font.bold: true
        text: title
    }
    Text {
        text: subtitle
        anchors.top: title_text.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}