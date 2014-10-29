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
        active: settings.flipToStopRecitation && _recitations.isPlaying
        onFlipped: _recitations.stop()
    }

    MediaPlayer {
        id: audioPlayer
    }

    Recitations {
        id: _recitations
        player: audioPlayer
        settings: settings
//        data: _data
//        downloader: _downloader
// TODO:
//        onError: banner.show(qsTr("Failed to play recitation"))

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
