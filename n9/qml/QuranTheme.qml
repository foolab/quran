// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

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
    property color subtitleColor: textColor
    property color verseColor: textColor
    property color recitationHighlightColor: "red"
    property color translationTextColor: textColor
    property color buttonNormalColor: inNightMode ? "white" : "black"

    property color buttonHighlightColor: "blue"

    property int marginSmall: 10
    property int marginMedium: 16
    property int spacing: 20
    property int toolButtonSize: 60

    property int fontSizeSmall: 24
    property int fontSizeLarge: 32
    property color primaryColor: inNightMode ? "white" : "black"
    property string fontFamilyHeading: "Nokia Pure Text"
    property int itemSizeSmall: 60
    property int itemSizeLarge: 80
    property int characterKeySize: 60
    property color highlightColor: "blue"

    property color statusError: "red"
    property color statusDownloading: "yellow"
    property color statusInstalled: "green"

    property bool needsKeyboard: true
    property string keyboardCapslock: "image://theme/icon-m-capslock"
    property string keyboardEnter: "image://theme/icon-m-enter"
    property string keyboardBackspace: "image://theme/icon-m-backspace"

    property color keyboardPreviewTextColor: "white"
    property color keyboardPreviewBackgroundColor: "black"
    property bool platformUsableKeyboard: true

    property int minimumFontSize: 16
    property int maximumFontSize: 48
    property int minimumTranslationFontSize: minimumFontSize
    property int maximumTranslationFontSize: maximumFontSize
}
