// -*- qml -*-
import QtQuick 1.0

Column {
        id: col
        width: parent.width
        property bool shown: false

        visible: col._chapter != -1 && col._verse != -1 && translationsManager.enabled && (_settings.translationMode == 1 || col.shown && _settings.translationMode == 2)

        property int _verse: -1
        property int _chapter: -1

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
                source: "image://theme/" + theme.translationBorder
                visible: col.visible
        }

        Label {
                id: translation
                visible: col.visible
                width: parent.width
                font.family: _settings.translationFontFamily
                font.pointSize: _settings.translationFontSize
                color: _colors.verseColor
                horizontalAlignment: _settings.centerText ? Text.AlignHCenter : undefined

                Image {
                        anchors.fill: parent
                        source: "image://theme/" + theme.translationBackground
                        z: translation.z - 1
                }

                function resetText() {
                        if (col._chapter != -1 && col._verse != -1) {
                                translation.text = _data.secondaryText(col._chapter, col._verse);
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
