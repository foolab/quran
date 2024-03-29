// -*- qml -*-
/*
 * Copyright (c) 2011-2020 Mohammed Sameer <msameer@foolab.org>.
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
    anchors.bottomMargin: open ? 0 : audioPlayer.state != Quran.Stopped ? - (height / 3) : - (height / 2)

    Behavior on anchors.bottomMargin {
        NumberAnimation { duration: quranTheme.animations.medium }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        height: audioPlayer.state != Quran.Stopped ? quranTheme.toolBarHeight : 0
        spacing: (parent.width - (children.length * quranTheme.sizes.toolButton)) / (children.length + 1)
        visible: height > 0

        Behavior on height {
            NumberAnimation { duration: quranTheme.animations.fast }
        }

        ToolButton {
            icon: "image://icon/play.png"
            fillColor: quranTheme.quranColors.text
            enabled: audioPlayer.state == Quran.Paused
            onClicked: audioPlayer.resume()
        }

        ToolButton {
            icon: "image://icon/pause.png"
            fillColor: quranTheme.quranColors.text
            enabled: audioPlayer.state == Quran.Playing
            onClicked: audioPlayer.pause()
        }

        ToolButton {
            icon: "image://icon/stop.png"
            fillColor: quranTheme.quranColors.text
            onClicked: audioPlayer.stop()
            enabled: audioPlayer.state != Quran.Stopped
        }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        height: quranTheme.toolBarHeight
        spacing: (parent.width - (children.length * quranTheme.sizes.toolButton)) / (children.length + 1)

        ToolButton {
            icon: "image://icon/back.png"
            rotation: open ? 270 : 90
            fillColor: quranTheme.quranColors.text
            onClicked: open = !open
            Behavior on rotation {
                NumberAnimation { duration: quranTheme.animations.medium }
            }
        }

        ToolButton {
            icon: "image://icon/favorites.png"
            fillColor: quranTheme.quranColors.text
            onClicked: pushAnimated(Qt.resolvedUrl("FavoritesPage.qml"))
        }

        NumberLabel {
            width: quranTheme.sizes.toolButton
            height: parent ? parent.height : width
            number: settings.pageNumber + 1
            onClicked: pushAnimated(Qt.resolvedUrl("IndexPage.qml"))
            color: highlight ? quranTheme.colors.primaryHighlight : quranTheme.quranColors.text
        }

        ToolButton {
            icon: "image://icon/play.png"
            fillColor: quranTheme.quranColors.text
            enabled: root.canPlayAudio
            onClicked: audioPlayer.playPage(settings.pageNumber)
        }

        ToolButton {
            icon: "image://icon/playlist.png"
            fillColor: quranTheme.quranColors.text
            enabled: root.canPlayAudio
            onClicked: pushAnimated(Qt.resolvedUrl("PlaylistPage.qml"))
        }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        height: quranTheme.toolBarHeight
        spacing: (parent.width - (children.length * quranTheme.sizes.toolButton)) / (children.length + 1)

        ToolButton {
            icon: "image://icon/settings.png"
            fillColor: quranTheme.quranColors.text
            onClicked: pushAnimated(Qt.resolvedUrl("SettingsPage.qml"))
        }

        ToolButton {
            icon: "image://icon/search.png"
            fillColor: quranTheme.quranColors.text
            onClicked: pushAnimated(Qt.resolvedUrl("SearchPage.qml"))
        }
    }
}
