// -*- qml -*-
/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
            topMargin: quranTheme.sizes.marginMedium
            bottomMargin: quranTheme.sizes.marginMedium
        }

        Column {
            id: col
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: quranTheme.sizes.marginMedium
                rightMargin: quranTheme.sizes.marginMedium
            }

            spacing: quranTheme.sizes.spacing
            width: parent.width

            QuranPageHeader {
                width: parent.width
                title: qsTr("About")
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/logo.png"
                width: Math.min(aboutPage.width, aboutPage.height) / 4
                height: width
            }

            QuranLabel {
                text: qsTr("Holy Quran Version %1").arg(settings.version)
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: quranTheme.fonts.heading
                font.pixelSize: quranTheme.fonts.large
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                color: quranTheme.colors.primary
            }

            QuranLabel {
                text: _about.copyright
                font.pixelSize: quranTheme.fonts.small
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                textFormat: Text.RichText
                color: quranTheme.colors.primary
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
