// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

PageStackWindow {
        id: root
        width: controller.width
        height: controller.height

        FSMonitor {
                id: fsmon
        }

        Settings {
                id: settings
        }

        Downloader {
                id: downloader
        }

        DataProvider {
                id: quranData
        }

        Bookmarks {
                id: bookmarks
                settings: settings
        }

        NumberFormatter {
                id: formatter
                format: settings.numberFormat
        }

        Search {
                id: search
        }

        Colors {
                id: colors
                theme: settings.theme
                nightMode: settings.nightMode
        }

        Translations {
                id: translations
                settings: settings
                downloader: downloader
                data: quranData
        }

        PhoneFlipControl {
                id: flipControl
                active: settings.flipToStopRecitation && recitations.isPlaying
                onFlipped: recitations.stop()
        }

        Recitations {
                id: recitations
                settings: settings
                data: quranData
        }

        WindowController {
                id: controller
                orientation: settings.orientation
                fullScreen: settings.fullScreen
                Component.onCompleted: {
                        exposedContentRectChanged();
                        applyOrientation();
                        show();
                }
        }

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
                        settings.pageNumber = quranData.pageNumberForSuraAndAya(sura, aya);
                        changed();
                }

//                onSuraChanged: console.log("Sura " + sura);
//                onAyaChanged: console.log("Aya " + aya);
//                onYChanged: console.log("Y " + y);
        }

        Connections {
                target: settings
                onTextTypeChanged: {
                        var type = quranData.textType();
                        if (!quranData.setTextType(settings.textType)) {
                                textError.show();
                                settings.textType = type;
                        }
                }
        }

        InfoBanner {
                id: favoriteAdded
                text: qsTr("Favorite added");

                Component.onCompleted: {
                        bookmarks.bookmarkAdded.connect(show);
                }
        }

        InfoBanner {
                id: favoriteRemoved
                text: qsTr("Favorite removed");

                Component.onCompleted: {
                        bookmarks.bookmarkRemoved.connect(show);
                }
        }

        InfoBanner {
                id: favoritesCleared
                text: qsTr("Favorites cleared");

                Component.onCompleted: {
                        bookmarks.cleared.connect(show);
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
                id: translationsBanner
        }

        InfoBanner {
                id: playerError
        }

        Connections {
                target: recitations
                onError: { playerError.text = msg; playerError.show(); }
        }

        Connections {
                target: translations
                onInstalled: {
                        translationsBanner.text = qsTr("Installed %1").arg(translations.translationName(id))
                        translationsBanner.show();
                }

                onFailed: {
                        translationsBanner.text = qsTr("Failed to download %1").arg(translations.translationName(id));
                        translationsBanner.show();
                }

                onRemoved: {
                        translationsBanner.text = qsTr("Removed %1").arg(translations.translationName(id));
                        translationsBanner.show();
                }
        }

        Connections {
                target: fsmon
                onAvailableChanged: {
                        if (!fsmon.available) {
                                translations.stopDownloads();
                        }
                }
        }

        Component.onCompleted: {
                quranData.setTextType(settings.textType)
                pageStack.push("QuranPage", Qt.point(0, 0), true);
        }
}
