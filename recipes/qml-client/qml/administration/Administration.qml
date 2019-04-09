import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4

Item {
    GridLayout {
        anchors.centerIn: parent
        columns: 2
        columnSpacing: 10
        rowSpacing: 10

        Button {
            text: qsTr("Users")
            onClicked: stackView.push(Qt.resolvedUrl("Users.qml"))
        }
        Button {
            text: qsTr("Ingredients")
            onClicked: stackView.push(Qt.resolvedUrl("Ingredients.qml"))
        }
        Button {
            text: qsTr("Units")
            onClicked: stackView.push(Qt.resolvedUrl("Units.qml"))
        }
        Button {
            text: qsTr("Categories")
            onClicked: stackView.push(Qt.resolvedUrl("Categories.qml"))
        }
    }
}
