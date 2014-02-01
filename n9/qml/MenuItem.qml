// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: menuItem
        property alias text: label.text
//        smooth: true
//        radius: 20
        width: parent.width
        height: label.height * 2
        color: mouse.pressed ? _colors.menuPressedBackgroundColor : _colors.menuBackgroundColor

        border.width: 1
        border.color: _colors.menuBorderColor

        signal clicked

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: menuItem.clicked();
        }

        Label {
                id: label
                font.pointSize: 24
                anchors.margins: 10
                font.bold: true
                y: height/2
                x: 20

                color: mouse.pressed ? _colors.menuPressedTextColor : _colors.menuTextColor
        }
}
