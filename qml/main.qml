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

QuranWindow {
    id: root

    Settings {
        id: settings
    }

    Downloader {
        id: _downloader
    }

    DataProvider {
        id: _data
    }

    Bookmarks {
        id: _bookmarks
        settings: settings
    }

    Translations {
        id: translations
        dir: settings.translationsDir
        downloader: _downloader
        data: _data

        function loadAndSetDefault(t) {
            if (loadTranslation(t)) {
                settings.defaultTranslation = t
                return true
            } else {
                banner.showMessage(qsTr("Failed to load translation"))
                return false
            }
        }

        Component.onCompleted: {
            refresh()
        }

        onDownloadError: banner.showMessage(qsTr("Failed to download %1").arg(name))

        onRefreshed: {
            if (!loadTranslation(settings.defaultTranslation)) {
                // Default translation cannot be loaded. Let's try another:
                var t = findInstalledTranslation();
                if (t == '') {
                    return
                }

                loadAndSetDefault(t)
            }
        }
    }

    PhoneFlipControl {
        id: flipControl
        active: settings.flipToStopRecitation && audioPlayer.playing
        onFlipped: audioPlayer.stop()
    }


    function playAudio(mode, id) {
        if (!audioPlayer.play(mode, id)) {
            banner.showMessage(qsTr("Failed to start audio playback"))
        }
    }

    MediaPlayer {
        id: audioPlayer
        downloader: _downloader
        onError: banner.showMessage(qsTr("Failed to play recitation"))

        onPositionChanged: {
            pagePosition.setPosition(chapter, verse)
            recitationPosition.verse = verse
            recitationPosition.chapter = chapter
        }

        onPlayingChanged: {
            recitationPosition.verse = -1
            recitationPosition.chapter = -1
        }
    }

    Recitations {
        id: recitations
        player: audioPlayer
        dir: settings.recitationsDir

        Component.onCompleted: {
            refresh()
            loadRecitation(settings.defaultRecitation)
        }
    }

    QuranTheme {
        id: quranTheme
        inNightMode: settings.nightMode
    }

    FSMonitor {
        id: fsmon
        onAvailableChanged: {
            if (!fsmon.available) {
                // TODO: show banner
                translations.stopDownloads();
            }
        }
    }

    initialPage: Component {
        MainPage {}
    }

    Component.onCompleted: _data.setTextType(settings.textType)

    PagePosition {
        id: pagePosition
        signal changed

        function setPosition(chapter, verse) {
            pagePosition.chapter = chapter
            pagePosition.verse = verse
            settings.pageNumber = pagePosition.page
            // We cannot depend on the pageChanged() signal because it will
            // be emitted after we set the chapter and verse but before we assign
            // the pageNumber above so our delegates will miss it because the actual
            // page change will happen after the reaction to pageChanged completes
            pagePosition.changed()
        }
    }

    QtObject {
        id: recitationPosition
        property int verse: -1
        property int chapter: -1
    }

    Connections {
        target: settings
        onTextTypeChanged: {
            var type = _data.textType();
            if (!_data.setTextType(settings.textType)) {
                settings.textType = type;
            }
        }
    }

    QuranNotificationBanner {
        id: banner
    }
}
