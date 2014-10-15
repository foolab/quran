// -*- qml -*-
import QtQuick 1.1

Text {
        // Some brilliant decided to force left alignment for all Harmattan
        // Labels. Even for Arabic and since we don't know if our text is
        // Arabic or not, we have to stop using Label.
        font.family: "Nokia Pure Text"
        font.pixelSize: 24
        wrapMode: Text.WordWrap
        color: quranTheme.primaryColor

        // TODO: this is for RecitationSelector.qml,
        // TranslationSelector.qml and DownloadLabel.qml. Check if we can remove it.
        property bool fadeTruncation: false
        //    truncationMode: fadeTruncation ? TruncationMode.Fade : TruncationMode.None
}
