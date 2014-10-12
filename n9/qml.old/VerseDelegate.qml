// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

Column {
        id: col

        Label {
                id: label

                signal clicked

                width: parent.width
                font.family: _settings.fontFamily
                font.pointSize: _settings.fontSize
                color: _recitations.chapter == chapter && _recitations.verse == verse ? _colors.highlightColor : _colors.verseColor
                horizontalAlignment: _settings.centerText ? Text.AlignHCenter : Text.AlignHRight

                Rectangle {
                        anchors.fill: parent
                        color: _colors.backgroundColor
                        z: label.z - 1
                }

                onClicked: {
                        menu.visible = !menu.visible
                        if (!menu.visible && _settings.translationMode == 2) {
                                translation.shown = false;
                        }
                }

                Component.onCompleted: {
                        _settings.textTypeChanged.connect(populate);
                        _settings.numberFormatChanged.connect(populate);
                        label.populate();
                }

                MouseArea {
                        anchors.fill: parent
                        onClicked: label.clicked();
                }

                function populate() {
                        text = _data.text(chapter, verse)
                               + " (" + _formatter.number(verse + 1) + ")";
                }

        }

        QuranPageContextMenu {
                id: menu
                _verse: verse
                _chapter: chapter
        }

        QuranTranslationLabel {
                id: translation
                _chapter: chapter
                _verse: verse
        }
}
