import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.4

Rectangle {
    anchors.fill: parent
    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            TextField {
                id: searchTerm
                Layout.fillWidth: true
                onEditingFinished: console.log("BUMM")
            }
            Button {
                id: button
                text: qsTr("Search")
                onClicked: recipeSearchModel.search(searchTerm.text)
            }
        }
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: "grey"
            border.width: 2
            ListView {
                anchors.fill: parent
                anchors.margins: 2
                model: recipeSearchModel
                delegate: RecipeListDelegate { }
            }
        }
    }
}
