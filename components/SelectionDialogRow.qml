// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: rect
        width: view.width
        height: label.height * 2
        // TODO: dependency from components to qml
        color: mouse.pressed ? "lightsteelblue" : index == dialog.selectedIndex ? "steelblue" : _colors.backgroundColor

        Label {
                id: label
                width: parent.width - 20
                text: name
                anchors.verticalCenter: parent.verticalCenter
                // TODO: dependency from components to qml
                color: index == dialog.selectedIndex ? "white" : _colors.textColor
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
