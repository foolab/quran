// -*- qml -*-
import QtQuick 1.0

Rectangle {
        property alias text: label.text
        property alias horizontalAlignment: label.horizontalAlignment

        height: label.height + 16
        color: colors.pageTitleBackgroundColor

        Label {
                id: label
                font.pointSize: 28
                color: colors.pageTitleTextColor
                width: parent.width - 32
                anchors.centerIn: parent
        }
}
