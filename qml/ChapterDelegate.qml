// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranLabel {
    ChapterInfo {
        id: info
        chapter: section
    }

    anchors {
        right: parent.right
        rightMargin: quranTheme.marginMedium
        left: parent.left
        leftMargin: quranTheme.marginMedium
    }

    horizontalAlignment: Text.AlignRight
    verticalAlignment: Text.AlignVCenter
    text: info.fullName
    color: quranTheme.highlightColor
}
