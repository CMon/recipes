import QtQuick 2.0
import QtQuick.Layouts 1.0

Rectangle {
    height: 100
    RowLayout {
        spacing: 10
        anchors.fill: parent
        Image {
            width: 100
            height: 100
            source: image
        }
        Text {
            text: title
            elide: Text.ElideRight
            Layout.fillWidth: true
        }
    }
}
