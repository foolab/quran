// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: rect
        width: view.width
        height: label.height * 2
        // TODO: dependency from components to qml
        color: mouse.pressed ? colors.pressedColor : index == dialog.selectedIndex ? colors.selectionBackgroundColor : colors.backgroundColor

        property alias textHorizontalAlignment: label.horizontalAlignment

        Label {
                id: label
                width: parent.width - 20
                text: name
                anchors.verticalCenter: parent.verticalCenter
                // TODO: dependency from components to qml
                color: mouse.pressed ? colors.pressedTextColor : index == dialog.selectedIndex ? colors.selectionTextColor : colors.textColor
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
