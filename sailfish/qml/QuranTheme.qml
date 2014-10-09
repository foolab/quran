// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QtObject {
    property bool inNightMode
    property string stop: _settings.theme + "/" + "stop"
    property color chapterBorder: "#3f48cc"
    property color chapterBackground: inNightMode ? "gray" : "#bfe8f2"
    property color translationBorder: "#00a3e8"
    property color translationBackground: chapterBackground
    property color backgroundColor: inNightMode ? "black" : "white"
    property color textColor: inNightMode ? "white" : "black"
    property color titleColor: textColor
    property color subtitleColor: textColor
    property color verseColor: textColor
    property color highlightColor: "red"
    property color translationTextColor: textColor
    property color buttonNormalColor: inNightMode ? "white" : "black"

    property color buttonHighlightColor: Theme.highlightColor

    property int marginSmall: 10
    property int marginMedium: 16
    property int marginLarge: 20
    property int spacing: 20
}
