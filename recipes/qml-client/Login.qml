import QtQuick 2.2
import QtQuick.Controls 1.2

Column {
    id: column

    signal loggedIn

    Component.onCompleted: {
        error.visible = false;
        error.text = ""
        username.focus = true;
    }

    TextField {
        id: username
        width: appWindow.width / 3
        anchors.right: column.right
        placeholderText: qsTr("Username")
        onAccepted: nextItemInFocusChain().forceActiveFocus()
    }
    TextField {
        id: password
        width: appWindow.width / 3
        anchors.right: column.right
        placeholderText: qsTr("Password")
        echoMode: TextInput.Password
        onAccepted: btnLogin.login(username.text, password.text)
    }
    Label {
        id: error
        color: "#FF0000"
        visible: false
    }

    Button {
        id: btnLogin
        anchors.right: column.right
        isDefault: true

        text: qsTr("Login");
        onClicked: login(username.text, password.text)

        function login(user, pass) {
            var loginOk = UserService.login(user, pass);
            if (loginOk) {
                error.visible = false;
                loggedIn()
            } else {
                error.visible = true;
                error.text = qsTr("Could not log in");
            }
        }
    }
}
