// -*- qml -*-
import QtQuick 1.0

QtObject {
        id: colors

        property color textColor: _settings.nightMode ? "white" : "black"
        property color backgroundColor: _settings.nightMode ? "black" : "white"
        property color verseColor: _settings.nightMode ? "white" : "black"
        property color titleColor: _settings.nightMode ? "black" : "white"
        property color subtitleColor: _settings.nightMode ? "black" : "white"
        property color highlightColor: _settings.nightMode ? "red" : "red"
        property color faderColor: _settings.nightMode ? "black" : "steelblue"
        property color sectionColor: "#a3daf4" // QColor(163, 218, 244)
        property color col1: Qt.lighter(colors.sectionColor, 1.2)
        property color col2: Qt.lighter(colors.sectionColor, 1.3)
}
