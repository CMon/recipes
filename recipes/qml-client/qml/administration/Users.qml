import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.4

import "../"

Rectangle {
    anchors.fill: parent
    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Text {
                text: qsTr("Filter:")
            }
            TextField {
                id: searchTerm
                Layout.fillWidth: true
                onTextChanged: usersModel.filter(text)
            }
            ImageButton {
                id: refresh
                width: 20
                height: 20
                image: "../../images/refresh.svg"
                onClicked: usersModel.update()
            }
        }
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: "grey"
            border.width: 2
            TableView {
                anchors.fill: parent
                anchors.margins: 2
                model: usersModel
                TableViewColumn {
                    role: "login"
                    title: "Login"
                    width: 100
                }
                TableViewColumn {
                    role: "firstName"
                    title: "First Name"
                    width: 100
                }
                TableViewColumn {
                    role: "lastName"
                    title: "Last Name"
                    width: 100
                }
                TableViewColumn {
                    role: "permissions"
                    title: "Permissions"
                    width: 200
                }
                TableViewColumn {
                    role: "deleted"
                    title: "Deleted"
                    width: 50
                    delegate: CheckBox {
                        checked: styleData.value
                    }
                }
            }
        }
    }
}
