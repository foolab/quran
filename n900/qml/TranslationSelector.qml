// -*- qml-mode -*-
import QtQuick 1.0

// TODO: try to merge with menu and dialog
Item {
	    id: dialog

        signal accepted
        signal rejected

        z: 1000

        state: "close"

        states: [
        State {
                name: "open"
                PropertyChanges { target: content; opacity: 1.0 }
                PropertyChanges { target: fader; opacity: 0.3 }
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

        anchors.fill: parent

        function open() {
                state = "open";
        }

        function close() {
                state = "close";
        }

        MouseArea {
                enabled: dialog.state == "open"
                anchors.fill: parent
                onClicked: {
                        dialog.close();
                        dialog.rejected();
                }
        }

        ListView {
                id: content
                z: 1002
                anchors.centerIn: parent

                width: parent.width * 3/4
                height: Math.min(_translations.installed.length * 80, parent.height * 3/4)

                model: _translations.installed
                delegate: contentDelegate
                header: Rectangle {
                        width: label.width
                        height: label.height
                        color: "steelblue"
                        Label {
                                id: label
                                text: qsTr("Choose translation");
                                width: content.width
                                color: "white"
                                x: 10
                        }
                }
        }


        Rectangle {
                id: fader
                z: 1001
                anchors.fill: parent
                color: "steelblue"
        }

        Component {
                id: contentDelegate

                Rectangle {
                        color: mouse.pressed ? "steelblue" : "white"
                        width: content.width;
                        height: 80

                        MouseArea {
                                anchors.fill: parent
                                id: mouse
                                onClicked: {
                                        if (!_translations.load(modelData)) {
                                                translationError.show();
                                                dialog.rejected();
                                        }
                                        else {
                                                dialog.accepted();
                                        }

                                        dialog.close();
                                }
                        }

                        Label {
                                id: label
                                width: parent.width;
                                text: _translations.translationName(modelData);
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.bold: _translations.current == modelData
                        }
                }
        }
}
