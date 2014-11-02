// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Label {
    property string title
    text: title

    horizontalAlignment: Text.AlignHCenter

    anchors {
        verticalCenter: parent.verticalCenter
        right: parent.right
        left: parent.left
        rightMargin: quranTheme.marginMedium
        leftMargin: quranTheme.marginMedium
    }

    visible: parent.count == 0
}
