import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import CMon.ClientController 1.0

Rectangle {
    id: root
    signal loggedIn

    Component.onCompleted: {
        error.visible = false;
        error.text = ""
        username.focus = true;
    }

    Connections {
        target: UserService
        onIsLoggedInChanged: {
            if (UserService.isLoggedIn) {
                console.log("logged in")
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Text { text: qsTr("Host:") }
            TextField {
                id: host
                text: "localhost"
            }
            Text { text: qsTr("Port:") }
            SpinBox {
                id: port
                stepSize: 1
                from: 0
                to: 65535
                value: 8080
                editable: true
                textFromValue: function(value, locale) { return Number(value).toString(); }
            }
            CheckBox {
                id: secure
                checked: false
                text: qsTr("Secure")
            }
            Button {
                id: btnConnect
                text: qsTr("&Connect");
                onClicked: ClientController.connectToServer(host.text, port.value, secure.checked)
            }
        }

        RowLayout {
            Text { text: qsTr("Username:") }
            TextField {
                id: username
            }
            Text { text: qsTr("Password:") }
            TextField {
                id: password
                echoMode: TextInput.Password
            }
            Button {
                id: btnLogin
                text: qsTr("Login");
                onClicked: UserService.login(username.text, password.text)
            }
        }
        Label {
            id: error
            color: "#FF0000"
            visible: false
        }

        Text {
            id: spacer
            text: ""
            Layout.fillHeight: true
        }
    }
}
