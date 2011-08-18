// -*- qml-mode -*-
import QtQuick 1.0

Rectangle {
        id: downloadLabel
        property bool showProgress: true
        property alias text: label.text
        property alias progress: slider.value

        property bool showRm: true

        signal clicked
        signal removeClicked

        color: mouse.pressed ? "steelblue" : "white"

        width: slider.width + 20 + rm.width + 10
        height: slider.y + slider.height

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: parent.clicked()
        }

        Label {
                id: label
                x: 10
                font.pointSize: 22
                width: slider.width
                anchors.top: parent.top
                anchors.topMargin: 10
                color: mouse.pressed ? "white" : "black"
        }

        Slider {
                id: slider
                x: label.x
                opacity: showProgress ? 1.0 : 0.0
                interactive: false
                minimumValue: 0
                maximumValue: 100
                anchors.top: label.bottom
                color: parent.color
        }

        ToolButton {
                id: rm
                icon: "general_delete"
                opacity: showRm ? 1.0 : 0.0
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                color: parent.color
                onClicked: parent.removeClicked()
        }
}
