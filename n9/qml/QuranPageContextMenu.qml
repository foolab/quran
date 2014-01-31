// -*- qml -*-
import QtQuick 1.0

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
                        target: bookmarks
                        onBookmarkAdded: button.resetIcon();
                        onBookmarkRemoved: button.resetIcon();
                        onCleared: button.resetIcon();
                }

                icon: bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse) ? theme.favoritesRemove : theme.favoritesAdd

                function resetIcon() {
                        if (bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse)) {
                                icon = theme.favoritesRemove;
                        }
                        else {
                                icon = theme.favoritesAdd;
                        }
                }

                onClicked: { bookmarks.isBookmarked(contextMenu.chapter, contextMenu.verse) ? bookmarks.remove(contextMenu.chapter, contextMenu.verse) : bookmarks.add(contextMenu.chapter, contextMenu.verse); }
        }

        ToolButton {
                icon: theme.translations
                enabled: settings.translationMode == 2 && fsmon.available
                onClicked: translation.shown = !translation.shown;
        }

        ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                icon: theme.playVerse
                enabled: settings.recitationMode != 0 && fsmon.available
                onClicked: recitations.play(contextMenu.chapter, contextMenu.verse);
        }

        ToolButton {
                id: playPage
                icon: theme.playPage
                enabled: settings.recitationMode != 0 && fsmon.available
                onClicked: recitations.playPage(settings.pageNumber);
        }

        ToolButton {
                id: playChapter
                icon: theme.playChapter
                enabled: settings.recitationMode != 0 && fsmon.available
                onClicked: recitations.playChapter(contextMenu.chapter);
        }

        ToolButton {
                id: stop
                icon: theme.stop
                onClicked: recitations.stop();
                enabled: recitations.isPlaying
        }
}
