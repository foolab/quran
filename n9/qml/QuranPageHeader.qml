// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Image {
        property alias title: label.text
        source: theme.inverted ? "image://theme/meegotouch-panel-inverted-background-pressed" : "image://theme/meegotouch-panel-background-pressed"

        QuranLabel {
                id: label
                font.pixelSize: quranTheme.fontSizeLarge
                font.family: quranTheme.fontFamilyHeading
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: quranTheme.marginMedium
        }
}
