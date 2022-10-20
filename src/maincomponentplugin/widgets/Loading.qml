import QtQuick 2.11
import QtQuick.Controls 2.4
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