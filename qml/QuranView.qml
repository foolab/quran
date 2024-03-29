// -*- qml -*-
/*
 * Copyright (c) 2011-2020 Mohammed Sameer <msameer@foolab.org>.
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

QuranListView {
    property int page: _data.pageCount - 1 - index
    id: view
    width: ListView.view.width
    height: ListView.view.height

    // Work around a null pointer dereference in Silica.
    interactive: ListView.isCurrentItem
    highlightFollowsCurrentItem: true
    highlightMoveDuration: 500

    // Needed to make flicking reliable with SilicaFlickable
    pressDelay: 0

    PageInfo {
        id: info
        page: view.page
    }

    header: Item {
        width: parent.width
        height: Math.max(verse.height, part.height)
        PartInfo {
            id: partInfo
            part: info.part
        }

        ChapterList {
            id: verse
            anchors.left: parent.left
            anchors.leftMargin: quranTheme.sizes.marginSmall
            chapters: info.chapters
        }

        QuranLabel {
            id: part
            anchors.right: parent.right
            anchors.rightMargin: quranTheme.sizes.marginSmall
            text: partInfo.name
            color: quranTheme.quranColors.text
        }
    }

    model: QuranViewModel {
        page: view.page
    }

    delegate: Loader {
        id: loader
        property int _chapter: chapter
        property int _verse: verse
        property bool _isFirstChapter: isFirstChapter
        width: parent.width
        sourceComponent: verse == -1 ? chapterDelegate : verseDelegate
    }

    Component {
        id: chapterDelegate
        BorderLabel {
            ChapterInfo {
                id: info
                chapter: _chapter
            }

            addTopMargin: !_isFirstChapter
            horizontalAlignment: Text.AlignHCenter
            borderColor: quranTheme.quranColors.chapterBorder
            backgroundColor: quranTheme.quranColors.chapterBackground
            textColor: quranTheme.quranColors.chapterTitle
            text:  qsTr("%1%2%3").arg(info.fullName)
                .arg(info.hasBasmala ? "\n" : "")
                .arg(info.hasBasmala ? _data.basmala : "")
            font {
                family: settings.fontFamily
                pixelSize: settings.fontSize
            }
        }
    }

    Component {
        id: verseDelegate

        Column {
            id: column

            anchors {
                left: parent ? parent.left : undefined
                right: parent ? parent.right : undefined
                rightMargin: quranTheme.sizes.marginSmall
                leftMargin: quranTheme.sizes.marginSmall
            }

            TextSupplier {
                id: supplier
                chapter: _chapter
                verse: _verse
                data: _data
            }

            QuranVerseLabel {
                id: label
                width: parent.width
                color: recitationPosition.chapter == _chapter && recitationPosition.verse == _verse ? quranTheme.quranColors.highlight : quranTheme.quranColors.text
                textSupplier: supplier
                verse: _verse
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: quranTheme.sizes.spacing

                ToolButton {
                    BookmarkItem {
                        id: checker
                        bookmarks: _bookmarks
                        bookmark: _bookmarks.serialize(_chapter, _verse)
                    }

                    icon: checker.isBookmarked ? "image://icon/favorite-selected.png" : "image://icon/favorite-unselected.png"
                    fillColor: quranTheme.quranColors.text
                    onClicked: checker.toggle()
                }

                ToolButton {
                    icon: "image://icon/translation.png"
                    fillColor: quranTheme.quranColors.text
                    visible: settings.translationsHidden
                    onClicked: translation.shown = !translation.shown;
                }

                ToolButton {
                    icon: "image://icon/play.png"
                    fillColor: quranTheme.quranColors.text
                    visible: root.canPlayAudio
                    onClicked: audioPlayer.playVerse(_chapter, _verse)
                }
            }

            QuranTranslationLabel {
                id: translation
                textSupplier: supplier
            }
        }
    }

    Connections {
        target: pagePosition
        onChanged: scrollRequest()
    }

    function scrollRequest() {
        if (!pagePosition.isValid ||
            view.page != pagePosition.page) {
            // Not for us.
            return;
        }

        var target = view.model.findIndex(pagePosition.chapter, pagePosition.verse)
        if (target != -1) {
            view.currentIndex = target
        }
    }
}
