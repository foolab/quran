// -*- qml -*-
import QtQuick 1.0

Rectangle {
        property int minimumValue: 0
        property int maximumValue: 0
        property int value: 0

        color: colors.progressBarBackgroundColor

        border.width: 1
        border.color: colors.progressBarBorderColor
        width: 350
        height: 10

        Rectangle {
                x: 0
                anchors.verticalCenter: parent.verticalCenter
                color: colors.progressBarColor
                width: parent.width * (parent.value - parent.minimumValue)/(parent.maximumValue - parent.minimumValue)
                height: parent.height
        }
}
