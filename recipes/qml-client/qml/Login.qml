import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import CMon.ClientController 1.0

Rectangle {
    id: root
    signal loggedIn()
    signal loggedOut()

    Component.onCompleted: {
        error.visible = false;
        error.text = ""
        username.focus = true;

        if (ClientController.isDebugBuild()) {
            username.text = "admin"
            password.text = "admin"
            btnLogin.focus = true;
        }
    }

    Connections {
        target: UserService
        onIsLoggedInChanged: {
            if (UserService.isLoggedIn) {
                root.loggedIn();
            } else {
                root.loggedOut();
            }
        }
    }

    ColumnLayout {
        anchors.centerIn: parent

        GridLayout {
            columns: 2

            Text { text: qsTr("Server:") }
            ComboBox {
                id: host
                editable: true
                model: [ "ws://localhost:8080", "wss://localhost:8080"] // improve this by reading/writing to a QSettings file
            }

            Text { text: qsTr("Username:") }
            TextField {
                id: username
            }

            Text { text: qsTr("Password:") }
            TextField {
                id: password
                echoMode: TextInput.Password
            }

            Text {}
            Button {
                id: btnLogin
                text: qsTr("Login");
                onClicked: {
                    if (ClientController.connectToServer(host.currentText, true)) {
                        UserService.login(username.text, password.text)
                    } else {
                        error.text = qsTr("Could not connect to server");
                        error.visible = true
                    }
                }
            }

            Label {
                id: error
                color: "#FF0000"
                visible: false
                Layout.rowSpan: 2
            }
        }
        Text {
            id: spacer
            Layout.fillHeight: true
        }
    }
}
