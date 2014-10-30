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
        id: _translations
        settings: settings
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
// TODO:
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
        data: _data
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
        id: _recitations
        player: audioPlayer
        settings: settings

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
                _translations.stopDownloads();
            }
        }
    }

    initialPage: Component {
        MainPage {}
    }

    Component.onCompleted: _data.setTextType(settings.textType)

    QtObject {
        id: pagePosition

        property int sura: -1
        property int aya: -1

        signal changed

        function isValid() {
            return sura != -1 && aya != -1
        }

        function reset() {
            sura = -1
            aya = -1
        }

        function setPosition(sura, aya) {
            pagePosition.sura = sura
            pagePosition.aya = aya
            settings.pageNumber = _data.pageNumberForSuraAndAya(sura, aya)
            changed()
        }

//                onSuraChanged: console.log("Sura " + sura);
//                onAyaChanged: console.log("Aya " + aya);
//                onYChanged: console.log("Y " + y);
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
/*
// TODO:
        Connections {
                target: _recitations
                onError: { playerError.text = msg; playerError.show(); }
        }
*/
}
