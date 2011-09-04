// -*- qml -*-
import QtQuick 1.0

Item {
        signal previousClicked
        signal nextClicked

        id: navBar

        property int duration: 200

        function show() {
                s.restart();
        }

        SequentialAnimation {
                id: s
                ParallelAnimation {
                        PropertyAnimation { target: prev; to: 1.0; properties: "opacity"; duration: navBar.duration}
                        PropertyAnimation { target: next; to: 1.0; properties: "opacity"; duration: navBar.duration}
                        PropertyAnimation { target: nextImage; to: 0.2; properties: "opacity"; duration: navBar.duration}
                        PropertyAnimation { target: prevImage; to: 0.2; properties: "opacity"; duration: navBar.duration}
                }

                PauseAnimation { duration: 2000; }
                ParallelAnimation {
                        PropertyAnimation { target: prev; to: 0.0; properties: "opacity"; duration: navBar.duration}
                        PropertyAnimation { target: next; to: 0.0; properties: "opacity"; duration: navBar.duration}
                        PropertyAnimation { target: nextImage; to: 0.0; properties: "opacity"; duration: navBar.duration}
                        PropertyAnimation { target: prevImage; to: 0.0; properties: "opacity"; duration: navBar.duration}
                }
        }

        Image {
                id: nextImage
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: next.width
                source: "image://theme/" + theme.navigationBarBackground
                opacity: 0.0

                MouseArea {
                        anchors.fill: parent
                        onClicked: navBar.nextClicked();
                }
        }

        ToolButton {
                id: next
                icon: theme.next
                anchors.left: parent.left
                anchors.verticalCenter: nextImage.verticalCenter
                onClicked: navBar.nextClicked();
                z: nextImage.z + 1
                opacity: 0.0
        }

        Image {
                id: prevImage
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: prev.width
                source: "image://theme/" + theme.navigationBarBackground
                opacity: 0.0

                MouseArea {
                        anchors.fill: parent
                        onClicked: navBar.previousClicked();
                }
        }

        ToolButton {
                id: prev
                icon: theme.previous
                anchors.right: parent.right
                anchors.verticalCenter: prevImage.verticalCenter
                onClicked: navBar.previousClicked();
                z: prevImage.z + 1
                opacity: 0.0
        }
}
