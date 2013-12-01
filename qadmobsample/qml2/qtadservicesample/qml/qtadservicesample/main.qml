import QtQuick 2.0
import QtAdService 1.0

Rectangle {
    width: 360
    height: 360
    Text {
        text: "Ad service status: " + adService.status
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
    }

    Rectangle {
        width: parent.width * 0.3
        height: width * 0.4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.1
        color: mouse.pressed ? "red": "darkRed"
        MouseArea {
            id: mouse
            anchors.fill: parent
            onClicked: {
                adService.fetchAd();
            }
        }
    }

    AdService {
        id: adService
    }
}
