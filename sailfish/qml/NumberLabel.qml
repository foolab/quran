// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

// HACK: Seems the only way to listen to _settings.numberFormat is to alias it to a property!
Label {
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
                format: _settings.numberFormat
        }

        font.bold: true
        font.family: Theme.fontFamily

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
}
