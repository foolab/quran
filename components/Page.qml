// -*- qml -*-
import QtQuick 1.0

Rectangle {
	    // TODO: for every page: reset flick when the orientation changes
        id: page

        // TODO: dependency from components to qml :(
        color: _colors.backgroundColor

        anchors.fill: parent

        function isPortrait() {
                return height > width;
        }

        MouseArea {
                anchors.fill: parent
                z: 3000
                enabled: parent.state == "hidden" || parent.state == "disabled"
        }

        property ToolBar tools: null

        state: "hidden"

        states: [
        State {
                name: "shown"
                PropertyChanges { target: tools; opacity: 1.0 }
                },
        State {
                name: "shownNow"
                PropertyChanges { target: page; opacity: 1.0 }
                },
        State {
                name: "hidden"
                PropertyChanges { target: page; opacity: 0.0 }
              },
        State {
                name: "enabled"
        },
        State {
                name: "disabled"
        }
        ]

        transitions: [
        Transition {
                from: "shown"; to: "hidden"
                PropertyAnimation { properties: "opacity"; duration: 100 }
        },
        Transition {
                from: "hidden"; to: "shown"
                PropertyAnimation { properties: "opacity"; duration: 100 }
        }
        ]
}
