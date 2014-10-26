// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
    property alias minimumValue: progress.minimumValue
    property alias maximumValue: progress.maximumValue
    property alias value: progress.value

    ProgressBar {
        id: progress
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
    }
}
