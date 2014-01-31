// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: button
        signal clicked

        property alias text: label.text
        property alias font: label.font
        property alias elide: label.elide
        property alias textAlignment: label.horizontalAlignment
        property bool checked: false

        color: colors.buttonBackgroundColor
        border.width: 2
        border.color: colors.buttonBorderColor
        width: 300
        height: 50
        radius: 5
        smooth: true

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: button.clicked();
        }

        Label {
                id: label
                width: parent.width - (parent.border.width * 2)
                // TODO: dependency from components to qml
                color: mouse.pressed || checked ? colors.buttonSelectedOrPressedTextColor : colors.buttonTextColor
                anchors.centerIn: parent
                font.pointSize: 20

                horizontalAlignment: Text.AlignHCenter
        }
}
