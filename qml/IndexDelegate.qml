// -*- qml -*-
import QtQuick 2.0

ListDelegate {
    id: root

    property int number
    property alias text: label.text
    property bool highlight
    property alias showPlay: play.visible
    property alias showMore: more.visible
    property alias showNumber: numberLabel.visible

    signal playClicked
    signal moreClicked

    contentHeight: quranTheme.itemSizeLarge

    Item {
        width: parent.width
        height: quranTheme.itemSizeLarge

        NumberLabel {
            id: numberLabel
            number: root.number + 1
            width: quranTheme.itemSizeSmall
            height: width
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            color: root.highlight ? quranTheme.highlightColor : quranTheme.primaryColor

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
        }

        QuranLabel {
            id: label
            height: parent.height
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
                left: play.visible ? play.right : parent.left
                verticalCenter: parent.verticalCenter
            }

            onClicked: root.moreClicked()
        }

        ToolButton {
            id: play
            icon.source: highlight ? "image://icon/play.png?" + quranTheme.buttonHighlightColor : "image://icon/play.png?" + quranTheme.primaryColor

            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
            }

            onClicked: root.playClicked()
        }
    }
}
