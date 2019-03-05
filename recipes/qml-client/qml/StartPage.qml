import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4

Item {
    GridLayout {
        anchors.centerIn: parent
        columns: 2
        columnSpacing: 10
        rowSpacing: 10

        ImageButton {
            width: 100
            height: 100
            image: "../images/search.svg"
            onClicked: stackView.push(Qt.resolvedUrl("search/Search.qml"))
        }

        ImageButton {
            width: 100
            height: 100
            image: "../images/add.svg"
            onClicked: stackView.push(Qt.resolvedUrl("createRecipe/CreateRecipe.qml"))
        }

        ImageButton {
            width: 100
            height: 100
            image: "../images/edit.svg"
            onClicked: stackView.push(Qt.resolvedUrl("createRecipe/CreateRecipe.qml"))
        }

        ImageButton {
            width: 100
            height: 100
            image: "../images/admin.svg"
            onClicked: stackView.push(Qt.resolvedUrl("administration/Administration.qml"))
        }
    }
}
