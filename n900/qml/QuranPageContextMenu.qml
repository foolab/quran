// -*- qml -*-
import QtQuick 1.0

Row {
        id: contextMenu

        height: 0
        property int chapter: -1
        property int verse: -1
        opacity: 0.0

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
                anchors.verticalCenter: parent.verticalCenter
                icon: theme.recitations
                // TODO:
                onClicked: _recitations.play(contextMenu.chapter, contextMenu.verse);
        }
}
