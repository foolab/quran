// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    property alias caption: label.text
    height: width
    onClicked: keyboard.keyClicked(caption)
    onPressed: keyboard.keyPressed(caption)
    onReleased: keyboard.keyReleased(caption)
    onCanceled: keyboard.keyReleased(caption)

    Label {
        id: label
        anchors.fill: parent
        font.family: Theme.fontFamily
        font.pixelSize: Theme.fontSizeSmall
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
     }
}
