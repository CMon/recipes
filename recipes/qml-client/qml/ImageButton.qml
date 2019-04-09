import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
    id:root
    signal clicked()
    property alias image: image.source
    color: "transparent"

    Image {
        Component.onCompleted: {
            if(width > root.width) {
                width = root.width;
            }
            if (height > root.height) {
                height = root.height;
            }
        }

        anchors.centerIn: parent
        id: image
        MouseArea {
            anchors.fill: parent
            onClicked: root.clicked()
        }
    }
}
