// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Label {
    property bool truncateText: false
    wrapMode: Text.WordWrap
    renderType: Text.QtRendering
    font.pixelSize: quranTheme.fontSizeSmall

    elide: truncateText ? Text.ElideRight : Text.ElideNone
    color: quranTheme.primaryColor
}
