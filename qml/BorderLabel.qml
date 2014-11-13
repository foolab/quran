// -*- qml -*-
import QtQuick 2.0

Column {
    id: column

    property color borderColor
    property color backgroundColor
    property alias textColor: label.color
    property real borderHeight: 5
    property alias text: label.text
    property alias font: label.font
    property alias horizontalAlignment: label.horizontalAlignment

    anchors {
        left: parent.left
        right: parent.right
        rightMargin: quranTheme.marginSmall
        leftMargin: quranTheme.marginSmall
    }

    Rectangle {
        width: parent.width
        height: column.borderHeight
        color: column.borderColor
    }

    Rectangle {
        width: parent.width
        height: label.height
        color: column.backgroundColor

        QuranLabel {
            id: label
            width: parent.width
        }
    }

    Rectangle {
        width: parent.width
        height: column.borderHeight
        color: column.borderColor
    }
}
