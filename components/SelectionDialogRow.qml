// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: rect
        width: view.width
        height: label.height * 2
        color: mouse.pressed ? "lightsteelblue" : index == dialog.selectedIndex ? "steelblue" : "white"

        Label {
                id: label
                width: parent.width - 20
                text: name
                anchors.verticalCenter: parent.verticalCenter
                color: index == dialog.selectedIndex ? "white" : "black"
                x: 10
        }

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: {
                        __hack = true;
                        selectedIndex = index;
                        __hack = false;
                        accept();
                }
        }
}
