// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Row {
        id: contextMenu

        property int chapter: -1
        property int verse: -1
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

                Connections {
                        target: _bookmarks
                        onBookmarkAdded: button.resetIcon();
                        onBookmarkRemoved: button.resetIcon();
                        onCleared: button.resetIcon();
                }

                image: _bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse) ? theme.favoritesRemove : theme.favoritesAdd

                function resetIcon() {
                        if (_bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse)) {
                                image = theme.favoritesRemove;
                        }
                        else {
                                image = theme.favoritesAdd;
                        }
                }

                onClicked: {
                        if (_bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse)) {
                                _bookmarks.remove(contextMenu.chapter, contextMenu.verse)
                        } else {
                                _bookmarks.add(contextMenu.chapter, contextMenu.verse)
                        }
                }
        }

        ToolButton {
                image: theme.translations
                visible: _settings.translationMode == 2 && _fsmon.available
                onClicked: translation.shown = !translation.shown;
        }

        ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                image: theme.playVerse
                visible: _settings.recitationMode != 0 && _fsmon.available
                onClicked: _recitations.play(contextMenu.chapter, contextMenu.verse);
        }

        ToolButton {
                id: playPage
                image: theme.playPage
                visible: _settings.recitationMode != 0 && _fsmon.available
                onClicked: _recitations.playPage(_settings.pageNumber);
        }

        ToolButton {
                id: playChapter
                image: theme.playChapter
                visible: _settings.recitationMode != 0 && _fsmon.available
                onClicked: _recitations.playChapter(contextMenu.chapter);
        }

        ToolButton {
                id: stop
                image: theme.stop
                onClicked: _recitations.stop();
                visible: _recitations.isPlaying
        }
}
