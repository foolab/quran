// -*- qml -*-
import QtQuick 2.0

QuranBackgroundItem {
    property alias caption: label.text
    height: width
    onClicked: keyboard.keyClicked(caption)
    onPressed: keyboard.keyPressed(caption)
    onReleased: keyboard.keyReleased(caption)
    onCanceled: keyboard.keyReleased(caption)

    QuranLabel {
        id: label
        anchors.fill: parent
        font.pixelSize: theme.fontSizeSmall
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
     }
}
