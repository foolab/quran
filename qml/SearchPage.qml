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
    id: searchPage

    property string searchString
    property Item searchField

    function doSearch() {
        searchModel.setQuery(searchPage.searchString, settings.searchMatchWholeWords)
    }

    Component {
        id: headerDelegate

        Column {
            width: parent.width
            spacing: quranTheme.sizes.spacing

            QuranPageHeader {
                width: parent.width
                title: qsTr("Search")
            }

            QuranSearchField {
                id: field

                Component.onCompleted: searchPage.searchField = field

                anchors {
                    left: parent.left
                    leftMargin: quranTheme.sizes.marginMedium
                    right: parent.right
                    rightMargin: quranTheme.sizes.marginMedium
                }

                enterKeyText: qsTr("Search")
                onTextChanged: searchPage.searchString = text
                enableSoftwareInputPanel: quranTheme.platformUsableKeyboard
                onEnterKeyClicked: {
                    if (searchPage.searchString.length) {
                        searchPage.doSearch()
                    }
                }
            }

            QuranTextSwitch {
                anchors {
                    left: parent.left
                    leftMargin: quranTheme.sizes.marginMedium
                    right: parent.right
                    rightMargin: quranTheme.sizes.marginMedium
                }

                text: qsTr("Match whole words only");
                checked: settings.searchMatchWholeWords
                onCheckedChanged: settings.searchMatchWholeWords = checked
            }

            QuranButton {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Search")
                onClicked: {
                    if (searchPage.searchString.length) {
                        searchPage.doSearch()
                    }
                }
            }
        }
    }

    Component {
        id: sectionDelegate
        ChapterDelegate {}
    }

    Component {
        id: searchPageDelegate

        VerseDelegate {
            id: item
            _chapter: chapter
            _verse: verse
        }
    }

    QuranListView {
        id: view
        anchors.fill: parent
        anchors.bottomMargin: keyboard.visible ? keyboard.height : 0
        clip: keyboard.visible
        header: headerDelegate

        section.property: "chapter"
        section.criteria: ViewSection.FullString
        section.delegate: sectionDelegate

        delegate: searchPageDelegate

        model: SearchModel {
            id: searchModel
            onErrorChanged: {
                if (searchModel.error) {
                    banner.showMessage(searchModel.errorString)
                }
            }
        }
    }

    Rectangle {
        id: preview
        property alias text: label.text
        width: quranTheme.sizes.itemLarge
        height: width
        color: quranTheme.keyboardPreviewBackgroundColor
        visible: !quranTheme.platformUsableKeyboard && text != ""
        radius: width / 3
        smooth: true
        border {
            color: quranTheme.keyboardPreviewTextColor
            width: quranTheme.sizes.decorative
        }

        anchors {
            bottom: keyboard.top
            horizontalCenter: parent.horizontalCenter
        }

        QuranLabel {
            id: label
            anchors.fill: parent
            color: quranTheme.keyboardPreviewTextColor
            font.pixelSize: quranTheme.fonts.large
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Keyboard {
        id: keyboard
        anchors {
            bottom: parent.bottom
            right: parent.right
            left: parent.left
        }

        opacity: quranTheme.platformUsableKeyboard ? 0 : searchPage.searchField.focus ? 1.0 : 0
        visible: !quranTheme.platformUsableKeyboard && opacity > 0

        Behavior on opacity {
            NumberAnimation { duration: quranTheme.animations.medium }
        }

        onKeyPressed: {
            if (code != " ") {
                preview.text = code
            }
        }

        onKeyReleased: preview.text = ""
        onKeyClicked: searchField.text = searchField.text + code
        onBackspaceClicked: searchField.text = searchField.text.slice(0, -1)
        onEnterClicked: {
            if (searchPage.searchString.length) {
                searchPage.doSearch()
                searchPage.searchField.focus = false
            }
        }
    }

}
