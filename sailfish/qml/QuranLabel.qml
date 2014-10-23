// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Label {
    font.family: Theme.fontFamily
    wrapMode: Text.WordWrap
    property bool truncateText: false
    truncationMode: truncateText ? TruncationMode.Fade : TruncationMode.None
}
