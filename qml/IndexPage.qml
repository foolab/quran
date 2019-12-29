// -*- qml -*-
/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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
    property list<QtObject> tabs: [
        QtObject {
            property string name: qsTr("Chapter")
            property string longName: qsTr("Choose chapter")
            property Component component: chapterComponent
        },

        QtObject {
            property string name: qsTr("Part")
            property string longName: qsTr("Choose part")
            property Component component: partComponent
        },

        QtObject {
            property string name: qsTr("Page")
            property string longName: qsTr("Choose page")
            property Component component: pageComponent
        }
    ]

    QuranPageHeader {
        id: pageHeader

        width: parent.width
        title: tabs[mainView.currentIndex].longName
    }

    Row {
        id: tabSelector
        anchors.top: pageHeader.bottom
        width: parent.width
        Repeater {
            model: tabs
            delegate: QuranBackgroundItem {
                width: parent.width / tabs.length
                height: quranTheme.sizes.itemSmall
                onClicked: mainView.currentIndex = index
                    QuranLabel {
                        function _color() {
                            if (current) {
                                return highlighted ? quranTheme.colors.secondaryHighlight : quranTheme.colors.secondary
                            } else {
                                return highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
                            }

                        }

                        property bool current: index == mainView.currentIndex
                        anchors.fill: parent
                        color: _color()
                        text: name
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }

    function repositionView(view) {
        view.positionViewAtIndex(view.currentIndex, ListView.Center)
    }

    QuranListView {
        id: mainView
        clip: true
        anchors {
            top: tabSelector.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        model: tabs
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 1

        delegate: Loader {
            sourceComponent: component
            width: ListView.view.width
            height: ListView.view.height
        }
    }

    Component {
        id: chapterComponent

        QuranListView {
            PageInfo {
                id: info
                page: settings.pageNumber
            }

            id: chapterView
            model: _data.chapterCount
            highlightFollowsCurrentItem: false
            currentIndex: info.firstChapter
            onWidthChanged: repositionView(chapterView)
            onHeightChanged: repositionView(chapterView)
            Component.onCompleted: repositionView(chapterView)

            delegate: IndexDelegate {
                ChapterInfo {
                    id: info
                    chapter: index
                }
                number: index
                text: info.fullName
                highlight: ListView.isCurrentItem
                showPlay: root.canPlayAudio
                onClicked: {
                    pagePosition.setPosition(index, 0)
                    popPage()
                }

                onPlayClicked: {
                    pagePosition.setPosition(index, 0)
                    playAudio(MediaPlayback.PlayChapter, index)
                    popPage()
                }

                onMoreClicked: {
                    pushAnimated(Qt.resolvedUrl("VerseSelectionPage.qml"), {chapter: number})
                }
            }
        }
    }

    Component {
        id: partComponent

        QuranListView {
            PageInfo {
                id: pageInfo
                page: settings.pageNumber
            }

            id: partView
            model: _data.partCount
            anchors.fill: parent
            highlightFollowsCurrentItem: false
            currentIndex: pageInfo.part

            onWidthChanged: repositionView(partView)
            onHeightChanged: repositionView(partView)
            Component.onCompleted: repositionView(partView)

            delegate: IndexDelegate {
                PartInfo {
                    id: info
                    part: index
                }
                number: index
                text: info.name
                showPlay: root.canPlayAudio
                showMore: false
                highlight: ListView.isCurrentItem

                onClicked: {
                    settings.pageNumber = info.firstPage
                    popPage()
                }

                onPlayClicked: {
                    settings.pageNumber = info.firstPage
                    playAudio(MediaPlayback.PlayPart, index)
                    popPage()
                }
            }
        }
    }

    Component {
        id: pageComponent
        Item {
            Column {
                width: parent.width
                spacing: quranTheme.sizes.spacing
                anchors.centerIn: parent
                QuranLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: quranTheme.sizes.marginMedium
                        rightMargin: quranTheme.sizes.marginMedium
                    }

                    text: qsTr("Enter a page number between 1 and %1").arg(_data.pageCount)
                    font.pixelSize: quranTheme.fonts.large
                    color: quranTheme.colors.secondary
                }

                QuranTextField {
                    id: label
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    text: settings.pageNumber + 1
                    width: parent.width / 2
                    focus: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: IntValidator { bottom: 1; top: _data.pageCount }

                    font {
                        pixelSize: quranTheme.fonts.large
                        family: quranTheme.fonts.heading
                    }

                    onTextChanged: slider.value = parseInt(text)
                    enterKeyEnabled: acceptableInput
                    enterKeyText: qsTr("Go!")
                    onEnterKeyClicked: {
                        if (acceptableInput) {
                            settings.pageNumber = text - 1
                            popPage()
                        }
                    }
                }

                Row {
                    width: parent.width
                    spacing: quranTheme.sizes.spacing
                    ToolButton {
                        id: minus
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "image://icon/minus.png"
                        enabled: parseInt(slider.value) > slider.minimumValue
                        onClicked: slider.value = parseInt(slider.value) - 1
                    }

                    QuranSlider {
                        id: slider
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width - plus.width - minus.width - (2 * parent.spacing)
                        minimumValue: 1
                        maximumValue: _data.pageCount
                        value: settings.pageNumber + 1
                        onValueChanged: label.text = parseInt(value)
                    }

                    ToolButton {
                        id: plus
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "image://icon/plus.png"
                        enabled: parseInt(slider.value) < slider.maximumValue
                        onClicked: slider.value = parseInt(slider.value) + 1
                    }
                }

                QuranButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: label.acceptableInput
                    text: qsTr("Go")
                    onClicked: {
                        if (label.acceptableInput) {
                            settings.pageNumber = label.text - 1
                            popPage()
                        }
                    }
                }
            }
        }
    }
}
