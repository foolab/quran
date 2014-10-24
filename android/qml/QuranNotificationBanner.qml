import QtQuick 2.2

Rectangle {
    anchors {
        horizontalCenter: parent.horizontalCenter
        bottom: parent.bottom
        bottomMargin: quranTheme.marginMedium * 10
    }

    width: parent.width * 0.75
    height: quranTheme.itemSizeSmall
    color: quranTheme.textColor
    opacity: timer.running ? 1.0 : 0.0
    radius: height / 4
    smooth: true
    Behavior on opacity {
        NumberAnimation { duration: 250 }
    }

    function showMessage(text) {
        label.text = text
        timer.restart()
    }

    QuranLabel {
        id: label
        anchors {
            fill: parent
            margins: quranTheme.marginSmall
        }

        height: parent.height
        verticalAlignment: Text.AlignVCenter
        color: quranTheme.backgroundColor
        truncateText: true
        font.bold: true
    }

    Timer {
        id: timer
        interval: 3000
    }
}
