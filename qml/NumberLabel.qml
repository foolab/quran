// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranLabel {
    id: label

    property alias number: formatter.number

    text: formatter.formattedNumber

    property bool enableSignals: true
    signal clicked
    signal pressAndHold
    property bool highlight: mouse.pressed && mouse.containsMouse

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: label.clicked()
        onPressAndHold: label.pressAndHold()
        enabled: parent.enableSignals
    }

    NumberFormatter {
        id: formatter
        format: settings.numberFormat
    }

    font.bold: true

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
}
