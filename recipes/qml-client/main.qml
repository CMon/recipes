import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.12

import CMon.ClientController 1.0

ApplicationWindow {
    id: appWindow
    title: qsTr("Recipes Client")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Login { onLoggedIn: /*stackView.push(Qt.resolvedUrl("StartPage.qml"))*/ console.log("go to start page") }
        focus: true
        Keys.onReleased: {
            if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
                if (stackView.depth > 1) {
                    stackView.pop()
                    event.accepted = true
                }
            }
        }
    }

    footer: Text {
        text: ClientController.status
    }
}
