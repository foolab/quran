// -*- qml-mode -*-
import QtQuick 1.0

Rectangle {
        property int minimumValue: 0
        property int maximumValue: 0
        property int value: 0

        border.width: 1
        border.color: "steelblue"
        width: 350
        height: 10

        Rectangle {
                x: 0
                anchors.verticalCenter: parent.verticalCenter
                color: "steelblue"
                width: parent.width * (parent.value - parent.minimumValue)/(parent.maximumValue - parent.minimumValue)
                height: parent.height
        }
}
