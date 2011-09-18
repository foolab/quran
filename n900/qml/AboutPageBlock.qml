// -*- qml -*-
import QtQuick 1.0

Rectangle {
        property alias title: titleLabel.text
        property alias text: textLabel.text

        height: titleLabel.height + textLabel.height
        width: parent.width

        Label {
                id: titleLabel
                font.bold: true
        }

        Label {
                id: textLabel
                wrapMode: Text.WordWrap
                anchors.top: titleLabel.bottom
                width: parent.width
                textFormat: Text.RichText
        }
}
