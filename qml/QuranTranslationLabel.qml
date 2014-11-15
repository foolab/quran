// -*- qml -*-
import QtQuick 2.0

MouseArea {
    property bool shown: false
    property variant textSupplier

    width: parent.width
    height: label.height

    visible: opacity > 0
    opacity: label.text != "" && ((shown && settings.translationsHidden) || !settings.translationsHidden) ? 1 : 0

    Behavior on opacity {
        NumberAnimation { duration: 100 }
    }

    onClicked: {
        if (settings.translationsHidden) {
            shown = !shown
        }
    }

    BorderLabel {
        id: label
        borderColor: quranTheme.translationBorder
        backgroundColor: quranTheme.translationBackground
        textColor: quranTheme.translationTextColor
        text: textSupplier.secondaryText
        font.pixelSize: settings.translationFontSize
    }
}
