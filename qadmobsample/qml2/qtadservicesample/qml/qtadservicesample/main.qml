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

    Image {
        anchors.centerIn: parent
        source: adService.ad.imageUrl
    }

    Rectangle {
        width: parent.width * 0.3
        height: width * 0.4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.1
        color: mouse.pressed ? "red": "darkRed"
        Text {
            font.pixelSize: parent.height * 0.4
            color: "white"
            text: "Fetch"
            anchors.centerIn: parent
        }
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
        slotId: "PUT_YOUR_SLOT_ID_HERE"
        testMode: true
        platform: AdPlatformAdfonic {

        }
    }
}
