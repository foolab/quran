// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: item

    property bool shown: false
    property int verse: -1
    property int chapter: -1

    width: parent.width
    height: rect.height + 8
    visible: item.chapter != -1 && item.verse != -1 && translationsManager.enabled && (_settings.translationMode == 1 || item.shown && _settings.translationMode == 2)
    opacity: visible ? 1 : 0

    onChapterChanged: translation.resetText();
    onVerseChanged: translation.resetText();

    Behavior on opacity {
        NumberAnimation { duration: 100 }
    }

    Rectangle {
        id: rect

        y: 0
        height: childRect.height + 10
        width: parent.width
        color: "#00a3e8"

        Rectangle {
            id: childRect
            height: translation.height
            width: parent.width
            color: "#bfe8f2"
            y: 5

            Label {
                id: translation
                width: parent.width
                font.family: Theme.fontFamily
                font.pixelSize: _settings.translationFontSize
                wrapMode: Text.WordWrap
                color: _colors.verseColor
                horizontalAlignment: _settings.centerText ? Text.AlignHCenter : undefined

                function resetText() {
                    if (item.chapter != -1 && item.verse != -1) {
                        translation.text = _data.secondaryText(item.chapter, item.verse);
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (_settings.translationMode == 2) {
                            shown = !shown;
                        }
                    }
                }

                Connections {
                    target: translationsManager
                    onEnabledChanged: translation.resetText();
                }

                Component.onCompleted: {
                    _settings.defaultTranslationChanged.connect(resetText);
                    translation.resetText();
                }
            }
        }
    }
}
