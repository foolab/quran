// -*- qml -*-
import QtQuick 1.1

Text {
    // Some brilliant decided to force left alignment for all Harmattan
    // Labels. Even for Arabic and since we don't know if our text is
    // Arabic or not, we have to stop using Label.
    font.family: "Nokia Pure Text"
    font.pixelSize: quranTheme.fontSizeSmall
    wrapMode: Text.WordWrap
    color: quranTheme.primaryColor

    property bool truncateText: false
    elide: truncateText ? Text.ElideRight : Text.ElideNone
}
