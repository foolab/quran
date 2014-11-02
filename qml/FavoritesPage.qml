// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
    id: favoritesPage
    menu: pageMenu

    DeletePopup {
        id: popup
        onConfirmed: _bookmarks.clear()
    }

    Connections {
        target: _bookmarks
        onCleared: popPage()
    }

    Component {
        id: sectionDelegate

        ChapterDelegate {}
    }

    Component {
        id: favoritesPageDelegate

        VerseDelegate {
            id: item
            _chapter: chapter
            _verse: verse
            actions: [
                MenuAction {
                    text: qsTr("Remove")
                    onClicked: deleter.confirm(item, qsTr("Removing"), qsTr("Remove item?"))
                }
            ]

            DeleteItem {
                id: deleter
                onConfirmed: _bookmarks.remove(bookmark)
            }
        }
    }

    QuranPageMenu {
        id: pageMenu
        view: view
        actions: [
            MenuAction {
                text: qsTr("Clear")
                onClicked: popup.confirm(qsTr("Clearing"), qsTr("Clear all favorites?"))
            }
        ]
    }

    QuranListView {
        id: view
        anchors.fill: parent

        model: BookmarksModel {
            bookmarks: _bookmarks
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Favorites")
        }

        section.property: "chapter"
        section.criteria: ViewSection.FullString
        section.delegate: sectionDelegate
        delegate: favoritesPageDelegate

        QuranViewPlaceholder {
            text: qsTr("No favorites added.\nTap the star button to add or remove favorites")
            enabled: _bookmarks.empty
        }
    }
}
