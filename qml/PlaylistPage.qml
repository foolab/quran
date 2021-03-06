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

QuranPage {
    id: page
    NumberFormatter {
        id: formatter
        format: settings.numberFormat
    }

    PageInfo {
        id: currentPageInfo
        page: settings.pageNumber
    }

    QuranFlickable {
        anchors.fill: parent
        contentHeight: col.height

        Column {
            id: col
            width: parent.width
            spacing: quranTheme.sizes.spacing

            QuranPageHeader {
                width: parent.width
                title: qsTr("Recite")
            }

            Item {
                width: parent.width
                height: Math.max(chapterSelector.height, chapterButton.height)

                QuranComboBox {
                    id: chapterSelector

                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                    }

                    width: parent.width / 2
                    model: ChapterListModel {}
                    textRole: function(model) { return model.name }
                    label: qsTr("Recite chapter")
                    currentIndex: currentPageInfo.firstChapter
                }

                QuranButton {
                    id: chapterButton
                    anchors.right: parent.right
                    text: qsTr("Recite")
                    enabled: root.canPlayAudio
                    onClicked: {
                        pagePosition.setPosition(chapterSelector.currentIndex, 0)
                        audioPlayer.playChapter(chapterSelector.currentIndex)
                        popPage()
                    }
                }
            }

            Item {
                width: parent.width
                height: Math.max(partSelector.height, partButton.height)

                QuranComboBox {
                    id: partSelector

                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                    }

                    width: parent.width / 2
                    model: PartListModel {}
                    textRole: function(model) { return model.name }
                    label: qsTr("Recite part")
                    currentIndex: currentPageInfo.part
                }

                QuranButton {
                    id: partButton
                    anchors.right: parent.right
                    enabled: root.canPlayAudio
                    text: qsTr("Recite")
                    PartInfo {
                        id: partInfo
                        part: partSelector.currentIndex
                    }
                    PageInfo {
                        id: pageInfo
                        page: partInfo.firstPage
                    }
                    onClicked: {
                        pagePosition.setPosition(pageInfo.firstChapter, pageInfo.firstVerse)
                        audioPlayer.playPart(partSelector.currentIndex)
                        popPage()
                    }
                }
            }

            Row {
                width: parent.width
                QuranComboBox {
                    id: fromChapterSelector
                    width: parent.width / 2
                    model: ChapterListModel {}
                    textRole: function(model) { return model.name }
                    label: qsTr("From chapter")
                    currentIndex: currentPageInfo.firstChapter
                }

                QuranComboBox {
                    id: fromVerseSelector
                    width: parent.width / 2
                    model: fromChapterInfo.length
                    label: qsTr("Verse")
                    textRole: function(model) { return formatter.formatNumber(model.index + 1) }
                    currentIndex: 0
                    ChapterInfo {
                        id: fromChapterInfo
                        chapter: fromChapterSelector.currentIndex
                    }
                }
            }

            Row {
                width: parent.width
                QuranComboBox {
                    id: toChapterSelector
                    width: parent.width / 2
                    model: ChapterListModel {}
                    textRole: function(model) { return model.name }
                    label: qsTr("To chapter")
                    currentIndex: currentPageInfo.lastChapter
                }

                QuranComboBox {
                    id: toVerseSelector
                    width: parent.width / 2
                    model: toChapterInfo.length
                    label: qsTr("Verse")
                    textRole: function(model) { return formatter.formatNumber(model.index + 1) }
                    currentIndex: toChapterInfo.length - 1
                    ChapterInfo {
                        id: toChapterInfo
                        chapter: toChapterSelector.currentIndex
                    }
                }
            }

            QuranButton {
                text: qsTr("Recite")
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: ((fromChapterSelector.currentIndex * 1000) + fromVerseSelector.currentIndex <= (toChapterSelector.currentIndex * 1000) + toVerseSelector.currentIndex) && root.canPlayAudio

                onClicked: {
                        pagePosition.setPosition(fromChapterSelector.currentIndex, fromVerseSelector.currentIndex)
                        audioPlayer.playRange(fromChapterSelector.currentIndex, fromVerseSelector.currentIndex, toChapterSelector.currentIndex, toVerseSelector.currentIndex)
                        popPage()
                }
            }
        }
    }

}
