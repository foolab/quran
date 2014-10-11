// -*- qml -*-
import QtQuick 2.0

QuranBackgroundItem {
    id: root

    property int number
    property alias text: label.text
    property bool highlight
    property alias showPlay: play.visible
    property alias showMore: more.visible
    property alias showNumber: numberLabel.visible

    signal playClicked
    signal moreClicked

    height: theme.itemSizeSmall

    anchors {
        right: parent.right
        rightMargin: theme.marginMedium
        left: parent.left
        leftMargin: theme.marginMedium
    }

    NumberLabel {
        id: numberLabel
        number: root.number + 1
        width: theme.itemSizeSmall
        height: width
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        color: root.highlight ? theme.highlightColor : theme.primaryColor

        anchors {
            top: parent.top
            right: parent.right
        }
    }

    QuranLabel {
        id: label
        height: theme.itemSizeSmall
        color: root.highlight ? theme.highlightColor : theme.primaryColor
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter

        anchors {
            top: parent.top
            right: numberLabel.visible ? numberLabel.left : parent.right
            left: more.visible ? more.right : play.visible ? play.right : parent.left
        }
    }

    QuranButton {
        id: more
        width: theme.itemSizeSmall
        height: width
        text: "..."

        anchors {
            top: parent.top
            left: play.visible ? play.right : parent.left
        }

        onClicked: root.moreClicked()
    }

    ToolButton {
        id: play
        width: theme.itemSizeSmall
        icon.source: highlight ? "image://icon/play.png?" + theme.buttonHighlightColor : "image://icon/play.png?" + theme.primaryColor

        anchors {
            top: parent.top
            left: parent.left
        }

        onClicked: root.playClicked()
    }
}
