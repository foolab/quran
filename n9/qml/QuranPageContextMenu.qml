// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

Row {
        id: contextMenu

        property int _chapter: -1
        property int _verse: -1
        visible: false

        anchors.horizontalCenter: parent.horizontalCenter

        PropertyAnimation {
                id: showAnimation
                target: contextMenu
                properties: "opacity"
                from: 0
                to: 1
                duration: 100
        }

        PropertyAnimation {
                id: hideAnimation
                target: contextMenu
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

        ToolButton {
                id: button
                anchors.verticalCenter: parent.verticalCenter

                BookmarkItem {
                        id: checker
                        bookmarks: _bookmarks
                        bookmark: _bookmarks.serialize(contextMenu._chapter, contextMenu._verse)
                }

                icon: checker.isBookmarked ? theme.favoritesRemove : theme.favoritesAdd

                onClicked: checker.toggle()
        }

        ToolButton {
                icon: theme.translations
                enabled: _settings.translationMode == 2 && _fsmon.available
                onClicked: translation.shown = !translation.shown;
        }

        ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                icon: theme.playVerse
                enabled: _settings.recitationMode != 0 && _fsmon.available
                onClicked: _recitations.play(contextMenu._chapter, contextMenu._verse);
        }

        ToolButton {
                id: playPage
                icon: theme.playPage
                enabled: _settings.recitationMode != 0 && _fsmon.available
                onClicked: _recitations.playPage(_settings.pageNumber);
        }

        ToolButton {
                id: playChapter
                icon: theme.playChapter
                enabled: _settings.recitationMode != 0 && _fsmon.available
                onClicked: _recitations.playChapter(contextMenu._chapter);
        }

        ToolButton {
                id: stop
                icon: theme.stop
                onClicked: _recitations.stop();
                enabled: _recitations.isPlaying
        }
}
