// -*- qml-mode -*-
import QtQuick 1.0

// TODO: try to merge with menu.
Item {
        property alias titleText: titleLabel.text
        property alias message: messageLabel.text
        property alias acceptButtonText: yes.text
        property alias rejectButtonText: no.text

        signal accepted
        signal rejected

        z: 1000
	    id: dialog

        state: "close"

        states: [
        State {
                name: "open"
                PropertyChanges { target: content; opacity: 1.0 }
                PropertyChanges { target: fader; opacity: 0.9 }
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

        Item {
                id: content
                z: 1002
                anchors.centerIn: parent
                width: 400
                height: (no.y + no.height) - titleLabel.y

                Label {
                        id: titleLabel
                        font.pointSize: 36
                        font.bold: true
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
//                        style: Text.Outline
//                        styleColor: "grey"
                        smooth: true
                }

                Label {
                        id: messageLabel
                        font.pointSize: 26
                        font.bold: true
                        anchors.top: titleLabel.bottom
                        anchors.topMargin: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
//                        style: Text.Outline
//                        styleColor: "grey"
                        smooth: true
                }

                DialogButton {
                        id: yes
//                        text: qsTr("Yes");
                        anchors.top: messageLabel.bottom
                        anchors.topMargin: 30
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
                                dialog.close();
                                dialog.accepted();
                        }
                }

                DialogButton {
                        id: no
//                        text: qsTr("No");
                        anchors.top: yes.bottom
                        anchors.topMargin: 20
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
                                dialog.close();
                                dialog.rejected();
                        }
                }
        }


        Rectangle {
                id: fader
                z: 1001
                anchors.fill: parent
                color: "steelblue"
        }


}
