// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

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
                BookmarkItem {
                        id: checker
                        bookmarks: _bookmarks
                        bookmark: _bookmarks.serialize(contextMenu.chapter, contextMenu.verse)
                }

                // TODO: adding or removing a bookmark does not change this until we restart
                image: checker.isBookmarked ? theme.favoritesRemove : theme.favoritesAdd

                onClicked: checker.toggle()
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
