// -*- qml -*-
import QtQuick 2.0

Item {
    id: item

    property bool shown: false
    property int verse: -1
    property int chapter: -1
    property variant textSupplier

    width: parent.width
    height: rect.height + 8
    visible: item.chapter != -1 && item.verse != -1 && (settings.translationMode == 1 || (item.shown && settings.translationMode == 2))
    opacity: visible ? 1 : 0

    Behavior on opacity {
        NumberAnimation { duration: 100 }
    }

    Rectangle {
        id: rect

        y: 0
        height: childRect.height + 10
        width: parent.width
        color: quranTheme.translationBorder

        Rectangle {
            id: childRect
            height: translation.height
            width: parent.width
            color: quranTheme.translationBackground
            y: 5

            QuranLabel {
                id: translation
                width: parent.width
                font.pixelSize: settings.translationFontSize
                color: quranTheme.translationTextColor
                horizontalAlignment: settings.centerText ? Text.AlignHCenter : undefined
                text: textSupplier.secondaryText

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (settings.translationMode == 2) {
                            shown = !shown;
                        }
                    }
                }
            }
        }
    }
}
