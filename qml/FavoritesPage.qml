// -*- qml -*-
/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
        }
    }

    QuranPageMenu {
        id: pageMenu
        view: view
        actions: [
            MenuAction {
                text: qsTr("Clear")
                icon: "image://icon/clear.png"
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
