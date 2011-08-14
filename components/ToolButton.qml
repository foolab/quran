// -*- qml-mode -*-
import QtQuick 1.0

Rectangle {
        color: "steelblue"
        id: button
        property string icon: ""
        signal clicked

        width: 60
        height: 60

        onIconChanged: img.source = "image://theme/" + icon

        MouseArea {
                anchors.fill: parent
                onClicked: button.clicked();
        }

        Image {
                width: 50
                height: 50
                x: 5
                y: 5

                id: img
                smooth: true
        }
}
