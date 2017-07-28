// -*- qml -*-
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
        Component.onCompleted: {
            refresh()
            loadTranslation(settings.defaultTranslation)
        }

        onDownloadError: banner.showMessage(qsTr("Failed to download %1").arg(name))
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
