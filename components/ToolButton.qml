// -*- qml-mode -*-
import QtQuick 1.0

Rectangle {
        color: "steelblue"
        id: button
        property string icon: ""
        signal clicked

        opacity: enabled ? 1.0 : 0.0
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
                anchors.centerIn: parent

                id: img
                smooth: true
        }
}
