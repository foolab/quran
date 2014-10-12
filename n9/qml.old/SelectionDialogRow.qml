// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: rect
        width: view.width
        height: label.height * 2
        property alias pressed: mouse.pressed
        color: mouse.pressed ? _colors.pressedColor : selected ? _colors.selectionBackgroundColor : _colors.backgroundColor
        property alias textHorizontalAlignment: label.horizontalAlignment
        property bool selected: index == dialog.selectedIndex
        signal clicked()

        Label {
                id: label
                width: parent.width - 20
                text: name
                anchors.verticalCenter: parent.verticalCenter
                color: mouse.pressed ? _colors.pressedTextColor : rect.selected ? _colors.selectionTextColor : _colors.textColor
                x: 10
        }

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: {
                        __hack = true;
                        selectedIndex = index;
                        rect.clicked()
                        __hack = false;
                        accept();
                }
        }
}
