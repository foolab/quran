// -*- qml -*-
import QtQuick 2.0

QuranBackgroundItem {
        property alias title: titleLabel.text
        property alias text: textLabel.text

        height: titleLabel.height + textLabel.height
        width: parent.width

        QuranLabel {
                id: titleLabel
                font.family: quranTheme.fontFamilyHeading
                font.bold: true
                color: quranTheme.primaryColor
        }

        QuranLabel {
                id: textLabel
                anchors.top: titleLabel.bottom
                width: parent.width
                textFormat: Text.RichText
                color: quranTheme.primaryColor
        }
}
