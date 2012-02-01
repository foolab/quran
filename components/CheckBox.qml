// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: checkBox
        property bool checked: false

        width: tick.width + label.width + 30
        height: Math.max(label.height, tick.height);
        property alias text: label.text

        // TODO: dependency from components to qml
        color: mouse.pressed ? "steelblue" : _colors.backgroundColor

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: parent.checked = !parent.checked
        }

        Rectangle {
                id: tick

                width: 60
                height: 60
                radius: 10
                color: "lightsteelblue"
                border.width: 1
                border.color: "white"
                smooth: true
                anchors.verticalCenter: parent.verticalCenter

                Image {
                        anchors.fill: parent
                        // TODO: depends on external stuff
                        source: checkBox.checked ? "image://theme/" + theme.close : ""

                }
        }

        Label {
                id: label
                anchors.left: tick.right
                anchors.leftMargin: 20
                font.pointSize: 22
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                // TODO: dependency from components to qml
                color: _colors.textColor
        }
}
