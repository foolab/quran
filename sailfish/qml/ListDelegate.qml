// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {
    id: root

    property alias number: numberLabel.number
    property alias text: label.text
    property bool highlight
    property alias showPlay: play.visible
    property alias showNumber: numberLabel.visible

    signal playClicked

    contentHeight: Theme.itemSizeSmall

    NumberLabel {
        id: numberLabel
        width: Theme.itemSizeSmall
        height: Theme.itemSizeSmall
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        color: root.highlight ? Theme.highlightColor : Theme.primaryColor

        anchors {
            top: parent.top
            right: parent.right
        }
    }

    Label {
        id: label
        height: Theme.itemSizeSmall
        color: root.highlight ? Theme.highlightColor : Theme.primaryColor
        font.family: Theme.fontFamily
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter

        anchors {
            top: parent.top
            right: numberLabel.visible ? numberLabel.left : parent.right
            left: play.visible ? parent.left : undefined
        }
    }

    IconButton {
        id: play
        width: Theme.itemSizeSmall
        height: Theme.itemSizeSmall
        icon.source: "image://theme/icon-m-play"

        anchors {
            top: parent.top
            left: parent.left
        }

        onClicked: root.playClicked()
    }
}