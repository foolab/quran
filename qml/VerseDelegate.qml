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

ListDelegate {
    id: item
    property int _chapter
    property int _verse

    onClicked: {
        pagePosition.setPosition(chapter, verse)
        popPage()
    }

    DeleteItem {
        id: deleter
        onConfirmed: verseBookmark.toggle()
    }

    BookmarkItem {
        id: verseBookmark
        bookmarks: _bookmarks
        bookmark: _bookmarks.serialize(chapter, verse)
    }

    Row {
        width: parent.width
        QuranLabel {
            id: label
            width: parent.width - button.width

            TextSupplier {
                id: supplier
                chapter: _chapter
                verse: _verse
                data: _data
            }

            NumberFormatter {
                id: formatter
                format: settings.numberFormat
                number: verse + 1
            }

            font.family: settings.fontFamily
            font.pixelSize: settings.fontSize
            text: qsTr("(%1) %2").arg(formatter.formattedNumber).arg(supplier.primaryText)
            horizontalAlignment: Text.AlignRight
            color: quranTheme.primaryColor
        }

        ToolButton {
            id: button
            anchors {
                top: parent.top
                verticalCenter: undefined
            }

            icon.source: verseBookmark.isBookmarked ? highlight ? "image://icon/favorite-selected.png?" + quranTheme.buttonHighlightColor : "image://icon/favorite-selected.png?" + quranTheme.buttonNormalColor : highlight ? "image://icon/favorite-unselected.png?" + quranTheme.buttonHighlightColor : "image://icon/favorite-unselected.png?" + quranTheme.buttonNormalColor
            onClicked: {
                if (verseBookmark.isBookmarked) {
                    deleter.confirm(item, qsTr("Removing"), qsTr("Remove item?"))
                } else {
                    verseBookmark.toggle()
                }
            }
        }
    }
}
