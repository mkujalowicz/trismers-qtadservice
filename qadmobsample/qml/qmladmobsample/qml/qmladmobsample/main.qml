import QtQuick 2.0
import QtAdmob 1.0

Rectangle {
    width: 360
    height: 360
    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    Admob {

    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }
}
