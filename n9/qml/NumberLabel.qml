// -*- qml -*-
import QtQuick 1.0

// HACK: Seems the only way to listen to settings.numberFormat is to alias it to a property!
Label {
        id: label
        signal clicked

        MouseArea {
                anchors.fill: parent
                onClicked: label.clicked();
        }

        property int number: 0
        property int format: settings.numberFormat
        color: colors.numberLabeltextColor

        font.bold: true

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        function resetText() {
                text = formatter.number(number + 1);
        }

        onNumberChanged: resetText();
        onFormatChanged: resetText();
        Component.onCompleted: resetText();
}
