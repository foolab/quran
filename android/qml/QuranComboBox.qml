// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Column {
    property list<MenuAction> actions
    property alias label: text.text
    property alias currentIndex: combo.currentIndex

    width: parent.width

    QuranLabel {
        id: text

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        font.bold: true
        font.pixelSize: quranTheme.fontSizeSmall
    }

    ComboBox {
        id: combo

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        textRole: "text"
        model: actions
        height: quranTheme.itemSizeSmall
    }
}
