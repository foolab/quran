// -*- qml -*-
import QtQuick 1.0

Item {
        signal previousClicked
        signal nextClicked

        id: navBar

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

        Image {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: next.width
                source: "image://theme/" + theme.navigationBarBackground

                ToolButton {
                        id: next
                        icon: theme.next
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea {
                        anchors.fill: parent
                        onClicked: navBar.nextClicked();
                }
        }

        Image {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: prev.width
                source: "image://theme/" + theme.navigationBarBackground

                ToolButton {
                        id: prev
                        icon: theme.previous
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea {
                        anchors.fill: parent
                        onClicked: navBar.previousClicked();
                }
        }
}
