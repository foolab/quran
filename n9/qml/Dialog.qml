// -*- qml -*-
import QtQuick 1.0

// TODO: try to merge with menu.
Item {
        id: dialog

        property real faderOpacity: 0.9

        property alias title: titleBar.children
        property alias content: contentField.children
        property alias buttons: buttonRow.children

        signal accepted
        signal rejected

        z: 1000

        state: "close"

        states: [
        State {
                name: "open"
                PropertyChanges { target: content; opacity: 1.0 }
                PropertyChanges { target: fader; opacity: faderOpacity }
                },
        State {
                name: "close"
                PropertyChanges { target: content; opacity: 0.0 }
                PropertyChanges { target: fader; opacity: 0.0 }
              }
        ]

        transitions: [
        Transition {
                from: "open"; to: "close"
                PropertyAnimation { properties: "opacity"; duration: 200 }
        },
        Transition {
                from: "close"; to: "open"
                PropertyAnimation { properties: "opacity"; duration: 200 }
        }
        ]

        anchors.centerIn: parent
        width: parent ? parent.width : undefined
        height: parent ? parent.height : undefined

        function accept() {
                close();
                accepted();
        }

        function reject() {
                close();
                rejected();
        }

        function open() {
                state = "open";
        }

        function close() {
                state = "close";
        }

        function isOpen() {
                return state == "open";
        }

        MouseArea {
                enabled: dialog.state == "open"
                anchors.fill: parent
                onClicked: dialog.reject();
        }

        Item {
                id: content
                width: parent.width
                height: childrenRect.height
                anchors.centerIn: parent

                z: 1001

                Item {
                        id: titleBar
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width
                        height: childrenRect.height
                }

                Item {
                        id: contentField
	                    anchors.leftMargin: 20
	                    anchors.rightMargin: 20
                        anchors.top: titleBar.bottom
                        anchors.topMargin: 20
                        width: parent.width
                        height: childrenRect.height
                        anchors.horizontalCenter: parent.horizontalCenter
                }

                Item {
                        id: buttonRow
                        anchors.top: contentField.bottom
                        anchors.topMargin: 30
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width
                        height: childrenRect.height
                }
        }

        Rectangle {
                id: fader
                z: 999
                anchors.fill: parent
                // TODO: dependency from components to qml
                color: colors.faderColor
        }
}
