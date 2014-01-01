// -*- qml -*-
import QtQuick 2.0

Item {
        // TODO: We need padding in case of long text.
        id: banner
        z: 2000

        anchors.fill: parent

        state: "hidden"

        property alias text: label.text

        Rectangle {
                id: content

                MouseArea {
                        anchors.fill: parent
                        enabled: banner.state == "shown"
                        onClicked: banner.hide();
                }

                y: 50
                color: _colors.infoBannerBackgroundColor
                border.width: 2
                border.color: _colors.infoBannerBorderColor
                width: parent.width * 3/4
                height: label.height + 20
                anchors.horizontalCenter: parent.horizontalCenter

                Label {
                        width: parent.width
                        y: 10
                        id: label
                        font.pointSize: 28
                        color: _colors.infoBannerTextColor
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                }
        }

        function show() {
                state = "shown";
                timer.restart();
        }

        function hide() {
                state = "hidden";
                timer.stop();
        }

        states: [
        State {
                name: "shown"
                PropertyChanges { target: content; opacity: 1.0 }
                PropertyChanges { target: content; width: parent.width * 3/4 }
                PropertyChanges { target: content; height: label.height + 20 }
                },
        State {
                name: "hidden"
                PropertyChanges { target: content; opacity: 0.0 }
                PropertyChanges { target: content; width: 0 }
                PropertyChanges { target: content; height: 0 }
              }
        ]

        transitions: [
        Transition {
                from: "shown"; to: "hidden"
                PropertyAnimation { properties: "opacity,width,height"; duration: 200 }
        },
        Transition {
                from: "hidden"; to: "shown"
                PropertyAnimation { properties: "opacity,width,height"; duration: 200 }
        }
        ]

        Timer {
                id: timer
                interval: 3200
                repeat: false

                onTriggered: banner.hide();
        }
}