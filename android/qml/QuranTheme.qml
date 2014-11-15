// -*- qml -*-
import QtQuick 2.0
import QtQuick.Window 2.2

Item {
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

    property color buttonHighlightColor: highlightColor

    property int marginSmall: Screen.height / 100
    property int marginMedium: Screen.height / 80
    property int spacing: Screen.height / 50
    property int toolButtonSize: Screen.height / 15

    property int fontSizeSmall: Screen.height / 35
    property int fontSizeLarge: Screen.height / 25
    property color primaryColor: inNightMode ? "white" : "black"
    property string fontFamilyHeading: "Nokia Pure Text"
    property int itemSizeSmall: Screen.height / 12
    property int itemSizeLarge: Screen.height / 10
    property int characterKeySize: 60
    property color highlightColor: "gray"

    property color statusError: "red"
    property color statusDownloading: "yellow"
    property color statusInstalled: "green"

    property bool needsKeyboard: true
    property string keyboardCapslock: "image://theme/icon-m-capslock"
    property string keyboardEnter: "image://theme/icon-m-enter"
    property string keyboardBackspace: "image://theme/icon-m-backspace"

    property color keyboardPreviewTextColor: "white"
    property color keyboardPreviewBackgroundColor: "black"

    property int toolBarHeight: Screen.height / 12
    property bool platformUsableKeyboard: true

    property int minimumFontSize: Screen.height / 70
    property int maximumFontSize: Screen.height / 10
    property int minimumTranslationFontSize: minimumFontSize
    property int maximumTranslationFontSize: maximumFontSize
}
