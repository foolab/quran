// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

// HACK: Seems the only way to listen to _settings.numberFormat is to alias it to a property!
Label {
        id: label
        property bool enableSignals: true
        signal clicked
        signal pressAndHold

        MouseArea {
                anchors.fill: parent
                onClicked: label.clicked()
                onPressAndHold: label.pressAndHold()
                enabled: parent.enableSignals
        }

        property int number: 0
        property int format: _settings.numberFormat
        color: _colors.numberLabeltextColor

        font.bold: true

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        function resetText() {
                text = _formatter.number(number + 1);
        }

        onNumberChanged: resetText();
        onFormatChanged: resetText();
        Component.onCompleted: resetText();
}
