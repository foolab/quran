// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
    property alias checked: textSwitch.checked
    property alias text: label.text

    anchors {
        left: parent.left
        right: parent.right
        leftMargin: quranTheme.marginSmall
        rightMargin: quranTheme.marginSmall
    }

    height: quranTheme.itemSizeSmall

    QuranLabel {
        id: label
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: textSwitch.left
            left: parent.left
        }

        verticalAlignment: Text.AlignVCenter
        font.bold: true
        color: quranTheme.primaryColor
    }

    Switch {
        id: textSwitch

        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }
}
