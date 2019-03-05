import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
    id:root
    signal clicked()
    property alias image: image.source
    color: "transparent"

    Image {
        anchors.centerIn: parent
        id: image
        MouseArea {
            anchors.fill: parent
            onClicked: root.clicked()
        }
    }
}
