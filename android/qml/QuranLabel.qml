// -*- qml -*-
import QtQuick.Controls 1.2

Label {
    wrapMode: Text.WordWrap
//    font.family: Theme.fontFamily

    property bool truncateText: false
    elide: truncateText ? Text.ElideRight : Text.ElideNone

}
//TODO:
