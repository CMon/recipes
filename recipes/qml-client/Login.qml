import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

ColumnLayout {
    id: column
    width: Math.max(btnLogin.width, appWindow.width / 3)

    signal loggedIn

    Component.onCompleted: {
        error.visible = false;
        error.text = ""
        username.focus = true;
    }

    TextField {
        id: username
        Layout.fillWidth: true
        placeholderText: qsTr("Username")
    }
    TextField {
        id: password
        Layout.fillWidth: true
        placeholderText: qsTr("Password")
        echoMode: TextInput.Password
    }
    Label {
        id: error
        color: "#FF0000"
        visible: false
    }

    Button {
        id: btnLogin
        anchors.right: column.right

        text: qsTr("Login");
        onClicked: {
            // call remote here
            var loginOk = true;
            if (loginOk) {
                loggedIn()
            } else {
                error.visible = true;
                error.text = qsTr("Could not log in");
            }
        }
    }
}
