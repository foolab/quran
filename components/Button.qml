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

        color: "steelblue"
        border.width: 2
        border.color: "black"
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
                color: mouse.pressed || checked ? "darkgrey" : _colors.textColor
                anchors.centerIn: parent
                font.pointSize: 20

                horizontalAlignment: Text.AlignHCenter
        }
}
