// -*- qml -*-
import QtQuick 1.0

QtObject {
        property string menuIcon: _settings.theme + "/" + "menu"
        property string fullScreenView: _settings.theme + "/" + "full-screen-view"
        property string normalView: _settings.theme + "/" + "normal-view"
        property string translations: _settings.theme + "/" + "translations"
        property string favorites: _settings.theme + "/" + "favorites-add"
        property string favoritesAdd: _settings.theme + "/" + "favorites-add"
        property string favoritesRemove: _settings.theme + "/" + "favorites-remove"
        property string previous: _settings.theme + "/" + "previous"
        property string next: _settings.theme + "/" + "next"
        property string pageBack: _settings.theme + "/" + "page-back"

        property string toolBarBackgroundPortrait: _settings.theme + "/" + "tool-bar-background-portrait"
        property string toolBarBackgroundLandscape: _settings.theme + "/" + "tool-bar-background-landscape"
        property string toolBarBackground: isPortrait() ? toolBarBackgroundPortrait : toolBarBackgroundLandscape

        property string buttonBackgroundPortrait: _settings.theme + "/" + "button-background-portrait"
        property string buttonBackgroundLandscape: _settings.theme + "/" + "button-background-landscape"
        property string buttonBackground: isPortrait() ? buttonBackgroundPortrait : buttonBackgroundLandscape
        property string remove: _settings.theme + "/" + "remove"
        property string downloaded: _settings.theme + "/" + "downloaded"
        property string add: _settings.theme + "/" + "add"
        property string clear: _settings.theme + "/" + "clear"
        property string close: _settings.theme + "/" + "close"

        property string page: _settings.theme + "/" + "page"
        property string verse: _settings.theme + "/" + "verse"
        property string part: _settings.theme + "/" + "part"
        property string index: _settings.theme + "/" + "index"
        property string search: _settings.theme + "/" + "search"
        property string addTranslation: _settings.theme + "/" + "add-translation"

        property string recitations: _settings.theme + "/" + "recitations"
        property string addRecitation: _settings.theme + "/" + "add-recitation"

        property string stop: _settings.theme + "/" + "stop"
        property string playVerse: _settings.theme + "/" + "play-verse"
        property string playPage: _settings.theme + "/" + "play-page"
        property string playChapter: _settings.theme + "/" + "play-chapter"
        property string playPart: _settings.theme + "/" + "play-part"

//        property string translationBorderTop: "translation-border-top"
        property string translationBorder: _settings.theme + "/" + "translation-border"
        property string translationBackground: _settings.theme + "/" + "translation-background"
        property string navigationBarBackground: translationBackground

        property string chapterBorder: _settings.theme + "/" + "chapter-border"
        property string chapterBackground: translationBackground
}
