// -*- qml -*-
import QtQuick 1.0

QtObject {
        property string menuIcon: settings.theme + "/" + "menu"
        property string fullScreenView: settings.theme + "/" + "full-screen-view"
        property string normalView: settings.theme + "/" + "normal-view"
        property string translations: settings.theme + "/" + "translations"
        property string favorites: settings.theme + "/" + "favorites-add"
        property string favoritesAdd: settings.theme + "/" + "favorites-add"
        property string favoritesRemove: settings.theme + "/" + "favorites-remove"
        property string previous: settings.theme + "/" + "previous"
        property string next: settings.theme + "/" + "next"
        property string pageBack: settings.theme + "/" + "page-back"

        property string toolBarBackgroundPortrait: settings.theme + "/" + "tool-bar-background-portrait"
        property string toolBarBackgroundLandscape: settings.theme + "/" + "tool-bar-background-landscape"
        property string toolBarBackground: isPortrait() ? toolBarBackgroundPortrait : toolBarBackgroundLandscape

        property string buttonBackgroundPortrait: settings.theme + "/" + "button-background-portrait"
        property string buttonBackgroundLandscape: settings.theme + "/" + "button-background-landscape"
        property string buttonBackground: isPortrait() ? buttonBackgroundPortrait : buttonBackgroundLandscape
        property string remove: settings.theme + "/" + "remove"
        property string downloaded: settings.theme + "/" + "downloaded"
        property string add: settings.theme + "/" + "add"
        property string clear: settings.theme + "/" + "clear"
        property string close: settings.theme + "/" + "close"

        property string page: settings.theme + "/" + "page"
        property string verse: settings.theme + "/" + "verse"
        property string part: settings.theme + "/" + "part"
        property string index: settings.theme + "/" + "index"
        property string search: settings.theme + "/" + "search"
        property string addTranslation: settings.theme + "/" + "add-translation"

        property string recitations: settings.theme + "/" + "recitations"
        property string addRecitation: settings.theme + "/" + "add-recitation"

        property string stop: settings.theme + "/" + "stop"
        property string playVerse: settings.theme + "/" + "play-verse"
        property string playPage: settings.theme + "/" + "play-page"
        property string playChapter: settings.theme + "/" + "play-chapter"
        property string playPart: settings.theme + "/" + "play-part"

//        property string translationBorderTop: "translation-border-top"
        property string translationBorder: settings.theme + "/" + "translation-border"
        property string translationBackground: settings.theme + "/" + "translation-background"
        property string navigationBarBackground: translationBackground

        property string chapterBorder: settings.theme + "/" + "chapter-border"
        property string chapterBackground: translationBackground
}
