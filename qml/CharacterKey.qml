// -*- qml -*-
import QtQuick 2.0

QuranBackgroundItem {
    id: item
    property alias caption: label.text
    height: width
    onClicked: keyboard.keyClicked(caption)
    onPressed: keyboard.keyPressed(caption)
    onReleased: keyboard.keyReleased(caption)
    onCanceled: keyboard.keyReleased(caption)

    QuranLabel {
        id: label
        anchors.fill: parent
        font.pixelSize: quranTheme.fontSizeSmall
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: item.highlighted ? quranTheme.highlightColor : quranTheme.primaryColor
     }
}
