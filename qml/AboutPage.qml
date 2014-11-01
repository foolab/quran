// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
    id: aboutPage

    AboutData {
        id: _about
    }

    QuranFlickable {
        id: flick
        anchors.fill: parent
        contentHeight: col.height
        anchors {
            topMargin: quranTheme.marginMedium
            bottomMargin: quranTheme.marginMedium
        }

        Column {
            id: col
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: quranTheme.marginMedium
                rightMargin: quranTheme.marginMedium
            }

            spacing: quranTheme.spacing
            width: parent.width

            QuranPageHeader {
                width: parent.width
                title: qsTr("About")
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/logo.png"
                width: 160
                height: 160
            }

            QuranLabel {
                text: qsTr("Holy Quran Version %1").arg(settings.version)
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: quranTheme.fontFamilyHeading
                font.pixelSize: quranTheme.fontSizeLarge
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                color: quranTheme.primaryColor
            }

            QuranLabel {
                text: _about.copyright
                font.pixelSize: quranTheme.fontSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                textFormat: Text.RichText
                color: quranTheme.primaryColor
            }

            AboutPageBlock {
                title: qsTr("Contributors:")
                text: _about.contributors
            }

            AboutPageBlock {
                title: qsTr("Application license:")
                text: _about.appLicense
            }

            AboutPageBlock {
                title: qsTr("License for simplified text:")
                text: _about.textLicense
            }

            AboutPageBlock {
                title: qsTr("Translations license:")
                text: _about.transLicense
            }
        }
    }
}
