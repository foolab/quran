// -*- qml -*-
import QtQuick 1.0

Rectangle {
        property alias title: titleLabel.text
        property alias text: textLabel.text

        height: titleLabel.height + textLabel.height
        width: parent.width

        color: _colors.backgroundColor

        Label {
                id: titleLabel
                font.bold: true
                color: _colors.textColor
        }

        Label {
                id: textLabel
                wrapMode: Text.WordWrap
                anchors.top: titleLabel.bottom
                width: parent.width
                textFormat: Text.RichText
                color: _colors.textColor
        }
}
