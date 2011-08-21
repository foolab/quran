// -*- qml-mode -*-
import QtQuick 1.0

Rectangle {
        id: checkBox
        property bool checked: false

        width: tick.width + label.width + 30
        height: Math.max(label.height, tick.height);
        property alias text: label.text

        color: mouse.pressed ? "steelblue" : "white"

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
                color: "steelblue"
                smooth: true
                anchors.verticalCenter: parent.verticalCenter

                Image {
                        anchors.fill: parent
                        source: checkBox.checked ? "image://theme/widgets_tickmark_list" : ""
                }
        }

        Label {
                id: label
                anchors.left: tick.right
                anchors.leftMargin: 20
                font.pointSize: 22
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
        }
}
