// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

MouseArea {
    id: mouse
    Rectangle {
        anchors.fill: parent
        color: mouse.pressed ? quranTheme.inNightMode ? "#191919" : "#e5e5e5" : quranTheme.backgroundColor
    }
}
