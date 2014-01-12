// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

ApplicationWindow {
        id: root

        FSMonitor {
                id: _fsmon
        }

        Settings {
                id: _settings
        }

        Downloader {
                id: _downloader
        }

        DataProvider {
                id: _data
        }

        Bookmarks {
                id: _bookmarks
                settings: _settings
        }

        NumberFormatter {
                id: _formatter
                format: _settings.numberFormat
        }

        Search {
                id: _search
        }

        Colors {
                id: _colors
                theme: _settings.theme
                nightMode: _settings.nightMode
        }

        Translations {
                id: _translations
                settings: _settings
                downloader: _downloader
                data: _data
        }

        Recitations {
                id: _recitations
                settings: _settings
                data: _data
        }

        QuranTheme {
                id: theme
        }

        TranslationsManager {
                id: translationsManager
        }

        RecitationsManager {
                id: recitationsManager
        }

        initialPage: Component {
                MainPage {}
        }

        Component.onCompleted: _data.setTextType(_settings.textType)

	    // TODO: duplicate of Page::isPortrait()
        function isPortrait() {
                return height > width;
        }

        QtObject {
                id: pagePosition

                property int sura: -1
                property int aya: -1

                signal changed

                function isValid() {
                        return sura != -1 && aya != -1;
                }

                function reset() {
                        sura = -1;
                        aya = -1;
                }

                function setPosition(sura, aya) {
                        pagePosition.sura = sura;
                        pagePosition.aya = aya;
                        _settings.pageNumber = _data.pageNumberForSuraAndAya(sura, aya);
                        changed();
                }

//                onSuraChanged: console.log("Sura " + sura);
//                onAyaChanged: console.log("Aya " + aya);
//                onYChanged: console.log("Y " + y);
        }

        Connections {
                target: _settings
                onTextTypeChanged: {
                        var type = _data.textType();
                        if (!_data.setTextType(_settings.textType)) {
                                _settings.textType = type;
                        }
                }
        }

        Rectangle {
                id: banner
                width: parent.width
                height: Theme.itemSizeSmall
                color: Theme.highlightColor
                anchors.top: parent.top
                opacity: 0
                visible: opacity > 0

                Behavior on opacity {
                        NumberAnimation {}
                }

                function show(text) {
                        timer.restart()
                        label.text = text
                        banner.opacity = 1
                }

                Timer {
                        id: timer
                        interval: 3000
                        running: banner.opacity == 1
                        onTriggered: banner.opacity = 0
                }

                Label {
                        id: label
                        anchors.fill: parent
                        anchors.leftMargin: 20
                        color: Theme.secondaryColor
                        truncationMode: TruncationMode.Fade
                        verticalAlignment: Text.AlignVCenter
                }
        }

/*
        InfoBanner {
                id: favoriteAdded
                text: qsTr("Favorite added");

                Component.onCompleted: {
                        _bookmarks.bookmarkAdded.connect(show);
                }
        }

        InfoBanner {
                id: favoriteRemoved
                text: qsTr("Favorite removed");

                Component.onCompleted: {
                        _bookmarks.bookmarkRemoved.connect(show);
                }
        }

        InfoBanner {
                id: firstPageReached
                text: qsTr("This is the first page");
        }

        InfoBanner {
                id: lastPageReached
                text: qsTr("This is the last page");
        }

        InfoBanner {
                id: translationError
                text: qsTr("Failed to load the translation");
        }

        InfoBanner {
                id: recitationError
                text: qsTr("Failed to load the recitation");
        }

        InfoBanner {
                id: noTranslations
                text: qsTr("You need to download a translation first");
        }

        InfoBanner {
                id: noRecitations
                text: qsTr("You need to install a recitation first");
        }

        InfoBanner {
                id: massStorage
                text: qsTr("Cannot be used in mass storage mode.");
        }

        InfoBanner {
                id: translations
        }

        InfoBanner {
                id: playerError
        }

        Connections {
                target: _recitations
                onError: { playerError.text = msg; playerError.show(); }
        }

        Connections {
                target: _translations
                onInstalled: {
                        translations.text = qsTr("Installed ") + _translations.translationName(id);
                        translations.show();
                }

                onFailed: {
                        translations.text = qsTr("Failed to download ") + _translations.translationName(id);
                        translations.show();
                }

                onRemoved: {
                        translations.text = qsTr("Removed ") + _translations.translationName(id);
                        translations.show();
                }
        }
*/
        Connections {
                target: _fsmon
                onAvailableChanged: {
                        if (!_fsmon.available) {
                                _translations.stopDownloads();
                        }
                }
        }
}
