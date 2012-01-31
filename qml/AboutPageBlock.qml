// -*- qml -*-
import QtQuick 1.0

Rectangle {
        property alias title: titleLabel.text
        property alias text: textLabel.text

        height: titleLabel.height + textLabel.height
        width: parent.width

        color: colors.backgroundColor

        Label {
                id: titleLabel
                font.bold: true
                color: colors.textColor
        }

        Label {
                id: textLabel
                wrapMode: Text.WordWrap
                anchors.top: titleLabel.bottom
                width: parent.width
                textFormat: Text.RichText
                color: colors.textColor
        }
}
