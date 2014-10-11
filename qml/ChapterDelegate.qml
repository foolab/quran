import QtQuick 2.0
import Quran 1.0

QuranLabel {
        anchors {
                right: parent.right
                rightMargin: theme.marginMedium
                left: parent.left
                leftMargin: theme.marginMedium
        }

        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        text: _data.fullSuraName(section)
        color: theme.highlightColor
}
