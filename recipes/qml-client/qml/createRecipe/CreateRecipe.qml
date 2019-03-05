import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.4

Rectangle {
    anchors.centerIn: parent
    color: "red"

    FileDialog {
        id: imageFileDialog
        title: "Please choose an image"
        folder: shortcuts.home
        selectMultiple: false
    }

    ColumnLayout {
        Rectangle {
            width: 100
            height: 100
            Layout.fillWidth: true

            Image {
                id: mainImage
                anchors.fill: parent
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        imageFileDialog.open()
                        if (imageFileDialog.fileUrl) {
                            mainImage.source = imageFileDialog.fileUrl
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Text { text: qsTr("Name") }
            TextInput {
                id: recipeName
                Layout.fillWidth: true
            }
            Text { text: qsTr("Portions") }
            SpinBox {
                id: portions
                value: 1
                minimumValue: 1
                maximumValue: 100
                decimals: 0
                stepSize: 1
            }
        }

        GroupBox {
            Layout.fillWidth: true
            title: qsTr("Ingredients")

            ListModel {
                id: ingredientsModel
            }

            ColumnLayout {
                anchors.fill: parent
                ListView {
                    height: 100
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: ingredientsModel
                }

                RowLayout {
                    id: ingredientsButtonBox
                    Text { Layout.fillWidth: true }
                    Button {
                        text: qsTr("Add")
                    }
                    Button {
                        text: qsTr("Remove")
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            title: qsTr("Steps")

            ListModel {
                id: stepsModel
            }

            ColumnLayout {
                anchors.fill: parent
                ListView {
                    height: 100
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: stepsModel
                }

                RowLayout {
                    id: stepsButtonBox
                    Button {
                        text: qsTr("up")
                    }
                    Button {
                        text: qsTr("down")
                    }
                    Text { Layout.fillWidth: true }
                    Button {
                        text: qsTr("Add")
                    }
                    Button {
                        text: qsTr("Remove")
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Text { Layout.fillWidth: true }
            Button {
                text: qsTr("Save")
                onClicked: {
                    console.log("store the recipe with alle the above information")
                }
            }
        }
    }

}
