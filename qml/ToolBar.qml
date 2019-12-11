// -*- qml -*-
/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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
            SequentialAnimation on rotation {
                loops: Animation.Infinite
                NumberAnimation {
                    from: 0
                    to: 180
                    duration: quranTheme.animations.slow
                }
                PauseAnimation { duration: quranTheme.animations.slow }
                NumberAnimation {
                    from: 180
                    to: 360
                    duration: quranTheme.animations.slow
                }
                PauseAnimation { duration: quranTheme.animations.slow }
                running: audioPlayer.paused
                alwaysRunToEnd: true
            }

            icon: audioPlayer.playing ? "image://icon/pause.png" : "image://icon/play.png"
            fillColor: quranTheme.quranColors.text
            enabled: settings.recitationMode != 0 && recitations.installedCount > 0
            onClicked: {
                if (audioPlayer.playing) {
                    if (audioPlayer.paused) {
                        audioPlayer.resume()
                    } else {
                        audioPlayer.pause()
                    }
                } else {
                    playAudio(MediaPlayback.PlayPage, settings.pageNumber)
                }
            }
        }

        ToolButton {
            icon: "image://icon/stop.png"
            fillColor: quranTheme.quranColors.text
            onClicked: audioPlayer.stop()
            enabled: audioPlayer.playing
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

        ToolButton {
            icon: "image://icon/playlist.png"
            fillColor: quranTheme.quranColors.text
            enabled: settings.recitationMode != 0 && recitations.installedCount > 0
            onClicked: pushAnimated(Qt.resolvedUrl("PlaylistPage.qml"))
        }
    }
}
