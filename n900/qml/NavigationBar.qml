// -*- qml-mode -*-
import QtQuick 1.0

Item {
        signal previousClicked
        signal nextClicked

        id: navBar
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        opacity: 0.0

        function show() {
                s.restart();
        }

         SequentialAnimation {
                 id: s
                 PropertyAnimation { target: navBar; to: 1.0; properties: "opacity"; duration: 200 }
                 PauseAnimation { duration: 2000; }
                 PropertyAnimation { target: navBar; to: 0.0; properties: "opacity"; duration: 200 }
        }

        ToolButton {
                id: next
                icon: theme.next
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                onClicked: navBar.nextClicked();
        }

        ToolButton {
                id: prev
                icon: theme.previous
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onClicked: navBar.previousClicked();
        }
}
