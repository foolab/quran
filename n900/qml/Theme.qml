// -*- qml-mode -*-
import QtQuick 1.0

QtObject {
        property string menuIcon: "menu"
        property string fullScreenView: "full-screen-view"
        property string normalView: "normal-view"
        property string translations: "translations"
        property string favorites: "favorites-add"
        property string favoritesAdd: "favorites-add"
        property string favoritesRemove: "favorites-remove"
        property string previous: "previous"
        property string next: "next"
        property string pageBack: "page-back"

        property string toolBarBackgroundPortrait: "tool-bar-background-portrait"
        property string toolBarBackgroundLandscape: "tool-bar-background-landscape"
        property string toolBarBackground: isPortrait() ? toolBarBackgroundPortrait : toolBarBackgroundLandscape

        property string buttonBackgroundPortrait: "button-background-portrait"
        property string buttonBackgroundLandscape: "button-background-landscape"
        property string buttonBackground: isPortrait() ? buttonBackgroundPortrait : buttonBackgroundLandscape
        property string remove: "remove"
        property string downloaded: "downloaded"
        property string add: "add"
        property string clear: "clear"

//        property string translationBorderTop: "translation-border-top"
        property string translationBorder: "translation-border"
        property string translationBackground: "translation-background"
}
