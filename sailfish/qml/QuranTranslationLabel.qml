// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
        id: col
        width: parent.width
        property bool shown: false

        visible: col.chapter != -1 && col.verse != -1 && translationsManager.enabled && (_settings.translationMode == 1 || col.shown && _settings.translationMode == 2)

        property int verse: -1
        property int chapter: -1

        onChapterChanged: translation.resetText();
        onVerseChanged: translation.resetText();

        PropertyAnimation {
                id: showAnimation
                target: col
                properties: "opacity"
                from: 0
                to: 1
                duration: 100
        }

        PropertyAnimation {
                id: hideAnimation
                target: col
                properties: "opacity"
                from: 1
                to: 0
                duration: 100
        }

        onVisibleChanged: {
                if (visible) {
                        hideAnimation.stop();
                        showAnimation.start();

                }
                else {
                        showAnimation.stop();
                        hideAnimation.start();
                }
        }

        Image {
                id: borderTop
                width: parent.width
                height: 5
                source: "image://icon/" + theme.translationBorder
                visible: col.visible
        }

        Label {
                id: translation
                visible: col.visible
                width: parent.width
                font.family: Theme.fontFamily
                font.pixelSize: _settings.translationFontSize
                wrapMode: Text.WordWrap
                color: _colors.verseColor
                horizontalAlignment: _settings.centerText ? Text.AlignHCenter : undefined

                Image {
                        anchors.fill: parent
                        source: "image://icon/" + theme.translationBackground
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
                source: "image://icon/" + theme.translationBorder
                visible: col.visible
        }

        Item {
                id: separator
                width: parent.width
                height: 8
                visible: col.visible
        }
}
