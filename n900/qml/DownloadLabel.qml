// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: downloadLabel
        property bool showProgress: true
        property alias text: label.text
        property alias progress: slider.value
        property alias errorText: error.text

        property bool showInstalled: true

        property bool showError: false

        signal clicked

        color: mouse.pressed ? "steelblue" : "white"

        width: slider.width + 20 + installed.width + 10
        height: Math.max(slider.y + slider.height + 10, 100)

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: parent.clicked()
        }

        Label {
                id: label
                x: 10
                font.pointSize: 24
                width: slider.width
                anchors.top: parent.top
                anchors.topMargin: 10
                color: mouse.pressed ? "white" : "black"
        }

        ProgressBar {
                id: slider
                x: label.x
                opacity: showProgress ? 1.0 : 0.0
                minimumValue: 0
                maximumValue: 100
                anchors.top: label.bottom
                anchors.topMargin: 10
        }

        Label {
                id: error
                color: "red"
                anchors.fill: slider
                opacity: showError ? 1.0 : 0.0
        }

        ToolButton {
                id: installed
                icon: theme.downloaded
                opacity: showInstalled ? 1.0 : 0.0
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                color: parent.color
        }
}
