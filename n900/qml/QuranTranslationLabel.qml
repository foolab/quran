// -*- qml -*-
import QtQuick 1.0
import Label2 1.0

Column {
        id: col
        width: parent.width
        property bool shown: false

        visible: col.chapter != -1 && col.verse != -1 && translationsManager.enabled && (_settings.translationMode == 1 || col.shown && _settings.translationMode == 2)

        property int verse: -1
        property int chapter: -1

        onChapterChanged: translation.resetText();
        onVerseChanged: translation.resetText();

        Image {
                id: borderTop
                width: parent.width
                height: 5
                source: "image://theme/" + theme.translationBorder
                visible: col.visible
        }

        Label2 {
                id: translation
                visible: col.visible
                width: parent.width
                font.family: _settings.translationFontFamily
                font.pointSize: _settings.translationFontSize
	            color: _settings.verseColor
                center: _settings.centerText

                Image {
                        anchors.fill: parent
                        source: "image://theme/" + theme.translationBackground
                        z: translation.z - 1
                }

                function resetText() {
                        if (col.chapter != -1 && col.verse != -1) {
                                translation.text = _data.secondaryText(col.chapter, col.verse);
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

        Image {
                id: borderBottom
                width: parent.width
                height: 5
                source: "image://theme/" + theme.translationBorder
                visible: col.visible
        }

        Item {
                id: separator
                width: parent.width
                height: 8
                visible: col.visible
        }
}
