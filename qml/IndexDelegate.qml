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

    height: quranTheme.itemSizeSmall

    anchors {
        right: parent.right
        rightMargin: quranTheme.marginMedium
        left: parent.left
        leftMargin: quranTheme.marginMedium
    }

    NumberLabel {
        id: numberLabel
        number: root.number + 1
        width: quranTheme.itemSizeSmall
        height: width
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        color: root.highlight ? quranTheme.highlightColor : quranTheme.primaryColor

        anchors {
            top: parent.top
            right: parent.right
        }
    }

    QuranLabel {
        id: label
        height: quranTheme.itemSizeSmall
        color: root.highlight ? quranTheme.highlightColor : quranTheme.primaryColor
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
        width: quranTheme.itemSizeSmall
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
        icon.source: highlight ? "image://icon/play.png?" + quranTheme.buttonHighlightColor : "image://icon/play.png?" + quranTheme.primaryColor

        anchors {
            top: parent.top
            left: parent.left
        }

        onClicked: root.playClicked()
    }
}
