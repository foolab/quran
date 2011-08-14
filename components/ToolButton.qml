// -*- qml-mode -*-
import QtQuick 1.0

Rectangle {
        color: "steelblue"
        id: button
        property string icon: ""
        signal clicked

        width: button.enabled ? 60 : 0
        height: button.enabled ? 60 : 0

        onIconChanged: img.source = "image://theme/" + icon

        MouseArea {
                anchors.fill: parent
                onClicked: button.clicked();
        }

        Image {
                width: button.enabled ? 50 : 0
                height: button.enabled ? 50 : 0
                x: 5
                y: 5

                id: img
                smooth: true
        }
}
