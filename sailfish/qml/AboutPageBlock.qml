// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
        property alias title: titleLabel.text
        property alias text: textLabel.text

        height: titleLabel.height + textLabel.height
        width: parent.width

        Label {
                id: titleLabel
                font.family: Theme.fontFamilyHeading
                font.bold: true
                color: Theme.primaryColor
        }

        Label {
                id: textLabel
                wrapMode: Text.WordWrap
                anchors.top: titleLabel.bottom
                width: parent.width
                textFormat: Text.RichText
                color: Theme.primaryColor
                font.family: Theme.fontFamily
        }
}
