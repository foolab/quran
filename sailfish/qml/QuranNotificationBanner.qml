// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    anchors {
        horizontalCenter: parent.horizontalCenter
        top: parent.top
    }

    width: parent.width
    height: Theme.itemSizeSmall
//    color: Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
    color: Theme.highlightBackgroundColor
    opacity: timer.running ? 1.0 : 0.0
    Behavior on opacity {
        NumberAnimation { duration: 250 }
    }

    function showMessage(text) {
        label.text = text
        timer.restart()
    }

    QuranLabel {
        id: label
        width: parent.width
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        color: Theme.primaryColor
        truncateText: true
    }

    Timer {
        id: timer
        interval: 3000
    }
}
