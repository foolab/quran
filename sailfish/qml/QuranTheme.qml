// -*- qml -*-
import QtQuick 2.0

QtObject {
        property string translations: _settings.theme + "/" + "translations"
        property string favorites: _settings.theme + "/" + "favorites-add"
        property string favoritesAdd: _settings.theme + "/" + "favorites-add"
        property string favoritesRemove: _settings.theme + "/" + "favorites-remove"
        property string pageBack: _settings.theme + "/" + "page-back"
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
        property string recitations: _settings.theme + "/" + "recitations"
        property string stop: _settings.theme + "/" + "stop"
        property string playVerse: _settings.theme + "/" + "play-verse"
        property string playPage: _settings.theme + "/" + "play-page"
        property string playChapter: _settings.theme + "/" + "play-chapter"
        property string playPart: _settings.theme + "/" + "play-part"

        property string translationBorder: _settings.theme + "/" + "translation-border"
        property string translationBackground: _settings.theme + "/" + "translation-background"
        property string chapterBorder: _settings.theme + "/" + "chapter-border"
        property string chapterBackground: translationBackground
}
