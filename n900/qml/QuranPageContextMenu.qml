// -*- qml -*-
import QtQuick 1.0

Row {
        id: contextMenu

        property int chapter: -1
        property int verse: -1
        visible: false

        anchors.horizontalCenter: parent.horizontalCenter

        ToolButton {
                id: button
                anchors.verticalCenter: parent.verticalCenter

                Connections {
                        target: _bookmarks
                        onBookmarkAdded: button.resetIcon();
                        onBookmarkRemoved: button.resetIcon();
                        onCleared: button.resetIcon();
                }

                icon: _bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse) ? theme.favoritesRemove : theme.favoritesAdd

                function resetIcon() {
                        if (_bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse)) {
                                icon = theme.favoritesRemove;
                        }
                        else {
                                icon = theme.favoritesAdd;
                        }
                }

                onClicked: { _bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse) ? _bookmarks.remove(contextMenu.chapter, contextMenu.verse) : _bookmarks.add(contextMenu.chapter, contextMenu.verse); }
        }

        ToolButton {
                icon: theme.translations
                enabled: _settings.translationMode == 2 && _fsmon.available
                onClicked: translation.shown = !translation.shown;
        }

        ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                icon: theme.recitations
                enabled: _settings.recitationMode != 0 && _fsmon.available
                onClicked: _recitations.play(contextMenu.chapter, contextMenu.verse);
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
                onClicked: _recitations.playChapter(contextMenu.chapter);
        }

        ToolButton {
                id: stop
                icon: theme.stop
                onClicked: _recitations.stop();
                enabled: _recitations.isPlaying
        }
}
