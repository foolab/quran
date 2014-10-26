// -*- qml -*-
import QtQuick 1.0
import com.nokia.meego 1.0

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
