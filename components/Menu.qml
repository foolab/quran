// -*- qml -*-
import QtQuick 1.0

Item {
        z: 1001
	    id: menu
        anchors.fill: parent

        Rectangle {
                id: fader
                anchors.fill: parent
                // TODO: dependency from components to qml
                color: colors.faderColor
        }

        MouseArea {
                enabled: menu.state == "open"
                anchors.fill: parent
                onClicked: menu.close();
        }

        state: "close"

        states: [
        State {
                name: "open"
                PropertyChanges { target: fader; opacity: 0.3 }
                },
        State {
                name: "close"
                PropertyChanges { target: fader; opacity: 0.0 }
              }
        ]

        transitions: [
        Transition {
                from: "open"; to: "close"
                PropertyAnimation { properties: "y,opacity"; duration: 200 }
        },
        Transition {
                from: "close"; to: "open"
                PropertyAnimation { properties: "y,opacity"; duration: 200 }
        }
        ]

        function open() {
                state = "open";
        }

        function close() {
                state = "close";
        }
}
