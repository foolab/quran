// -*- qml -*-
import QtQuick 1.0

PageStackWindow {
        id: root
        Theme {
                id: theme
        }

        TranslationsManager {
                id: translationsManager
        }

        RecitationsManager {
                id: recitationsManager
        }

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
                                textError.show();
                                _settings.textType = type;
                        }
                }
        }

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
                id: favoritesCleared
                text: qsTr("Favorites cleared");

                Component.onCompleted: {
                        _bookmarks.cleared.connect(show);
                }
        }

        InfoBanner {
                id: textError
                text: qsTr("Failed to load the text");
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

        Connections {
                target: _fsmon
                onAvailableChanged: {
                        if (!_fsmon.available) {
                                _translations.stopDownloads();
                        }
                }
        }

        Component.onCompleted: {
                _data.setTextType(_settings.textType)
                pageStack.push("QuranPage", Qt.point(0, 0), true);
        }
}
