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

Column {
    width: parent.width

    property bool open: false
    anchors.bottom: parent.bottom
    anchors.bottomMargin: open ? 0 : - (height / 2)

    Behavior on anchors.bottomMargin {
        NumberAnimation { duration: quranTheme.animations.medium }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        height: quranTheme.toolBarHeight
        spacing: (parent.width - (5 * quranTheme.sizes.toolButton)) / 6

        ToolButton {
            icon: "image://icon/back.png"
            rotation: open ? 270 : 90
            onClicked: open = !open
            Behavior on rotation {
                NumberAnimation { duration: quranTheme.animations.medium }
            }
        }

        ToolButton {
            icon: "image://icon/favorites.png"
            onClicked: pushAnimated(Qt.resolvedUrl("FavoritesPage.qml"))
        }

        NumberLabel {
            width: quranTheme.sizes.toolButton
            height: parent ? parent.height : width
            number: settings.pageNumber + 1
            onClicked: pushAnimated(Qt.resolvedUrl("IndexPage.qml"))
            color: highlight ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
        }

        ToolButton {
            icon: "image://icon/play.png"
            enabled: settings.recitationMode != 0 && recitations.installedCount > 0
            onClicked: playAudio(MediaPlayer.PlayPage, settings.pageNumber)
        }

        ToolButton {
            icon: "image://icon/stop.png"
            onClicked: audioPlayer.stop()
            enabled: audioPlayer.playing
        }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        height: quranTheme.toolBarHeight
        spacing: (parent.width - (2 * quranTheme.sizes.toolButton)) / 3

        ToolButton {
            icon: "image://icon/settings.png"
            onClicked: pushAnimated(Qt.resolvedUrl("SettingsPage.qml"))
        }

        ToolButton {
            icon: "image://icon/search.png"
            onClicked: pushAnimated(Qt.resolvedUrl("SearchPage.qml"))
        }
    }
}
