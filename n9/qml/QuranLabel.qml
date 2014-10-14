// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Label {
    font.family: Theme.fontFamily

    // TODO: this is for RecitationSelector.qml,
    // TranslationSelector.qml and DownloadLabel.qml. Check if we can remove it.
    property bool fadeTruncation: false
//    truncationMode: fadeTruncation ? TruncationMode.Fade : TruncationMode.None
}
