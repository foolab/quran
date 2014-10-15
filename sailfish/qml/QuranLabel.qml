// -*- qml -*-
import Sailfish.Silica 1.0

Label {
    font.family: Theme.fontFamily

    property bool truncateText: false
    truncationMode: truncateText ? TruncationMode.Fade : TruncationMode.None
}
