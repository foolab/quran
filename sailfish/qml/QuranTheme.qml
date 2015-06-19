// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QtObject {
    property bool inNightMode
    property string stop: settings.theme + "/" + "stop"
    property color chapterBorder: "#3f48cc"
    property color chapterBackground: inNightMode ? "gray" : "#bfe8f2"
    property color translationBorder: "#00a3e8"
    property color translationBackground: chapterBackground
    property color backgroundColor: inNightMode ? "black" : "white"
    property color textColor: inNightMode ? "white" : "black"
    property color titleColor: textColor
    property color verseColor: textColor
    property color recitationHighlightColor: "red"
    property color translationTextColor: textColor
    property color buttonNormalColor: inNightMode ? "white" : "black"

    property color buttonHighlightColor: Theme.highlightColor

    property int marginSmall: 10
    property int marginMedium: 16
    property int spacing: 20
    property int toolButtonSize: Theme.itemSizeSmall

    property int fontSizeSmall: Theme.fontSizeSmall
    property int fontSizeLarge: Theme.fontSizeLarge
    property color primaryColor: Theme.primaryColor
    property string fontFamilyHeading: Theme.fontFamilyHeading
    property int itemSizeSmall: Theme.itemSizeSmall
    property int itemSizeLarge: Theme.itemSizeLarge
    property int characterKeySize: Theme.itemSizeExtraSmall
    property color highlightColor: Theme.highlightColor

    property color statusError: "red"
    property color statusDownloading: Theme.highlightColor
    property color statusInstalled: Theme.primaryColor

    property bool needsKeyboard: true
    property string keyboardEnter: "image://theme/icon-m-enter"
    property string keyboardBackspace: "image://theme/icon-m-backspace"

    property color keyboardPreviewTextColor: "white"
    property color keyboardPreviewBackgroundColor: "black"
    property bool platformUsableKeyboard: false

    property int minimumFontSize: Theme.fontSizeTiny
    property int maximumFontSize: Theme.fontSizeHuge
    property int minimumTranslationFontSize: minimumFontSize
    property int maximumTranslationFontSize: maximumFontSize
}
