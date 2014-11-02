// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

QuranLabel {
    property string title
    text: title

    horizontalAlignment: Text.AlignHCenter

    anchors {
        verticalCenter: parent.verticalCenter
        right: parent.right
        left: parent.left
        rightMargin: quranTheme.marginMedium
        leftMargin: quranTheme.marginMedium
    }

    visible: parent.count == 0
    color: quranTheme.primaryColor
}
