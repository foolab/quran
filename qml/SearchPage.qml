// -*- qml -*-
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
                        spacing: quranTheme.spacing

                        QuranPageHeader {
                                width: parent.width
                                title: qsTr("Search")
                        }

                        QuranSearchField {
                                id: field
                                Component.onCompleted: searchPage.searchField = field
                                anchors {
                                    left: parent.left
                                    leftMargin: quranTheme.marginMedium
                                    right: parent.right
                                    rightMargin: quranTheme.marginMedium
                                }

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
                                    leftMargin: quranTheme.marginMedium
                                    right: parent.right
                                    rightMargin: quranTheme.marginMedium
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
                        BookmarkItem {
                                id: verseBookmark
                                bookmarks: _bookmarks
                                bookmark: _bookmarks.serialize(chapter, verse)
                        }

                        DeleteItem {
                                id: deleter
                                onConfirmed: verseBookmark.toggle()
                        }

                        actions: [
                        MenuAction {
                                text: qsTr("Add to favorites")
                                visible: !verseBookmark.isBookmarked
                                onClicked: verseBookmark.toggle()
                        },
                        MenuAction {
                                text: qsTr("Remove from favorites")
                                visible: verseBookmark.isBookmarked
                                onClicked: deleter.confirm(item, qsTr("Removing"), qsTr("Remove item?"))
                        }
                        ]
                }
        }

        QuranListView {
                id: view
                anchors.fill: parent
                anchors.bottomMargin: keyboard.visible ? keyboard.height : 0
                clip: keyboard.visible
                header: headerDelegate

                QuranViewPlaceholder {
                        text: searchModel.errorString
                        enabled: searchModel.error
                }

                section.property: "chapter"
                section.criteria: ViewSection.FullString
                section.delegate: sectionDelegate

                delegate: searchPageDelegate

                model: SearchModel {
                    id: searchModel
                }
        }

        Rectangle {
            id: preview
            property alias text: label.text
            width: quranTheme.itemSizeLarge
            height: width
            color: quranTheme.keyboardPreviewBackgroundColor
            visible: !quranTheme.platformUsableKeyboard && text != ""
            radius: width / 3
            smooth: true
            border {
                    color: quranTheme.keyboardPreviewTextColor
                    width: 1
            }
            anchors {
                bottom: keyboard.top
                horizontalCenter: parent.horizontalCenter
            }

            QuranLabel {
                color: quranTheme.keyboardPreviewTextColor
                id: label
                anchors.fill: parent
                font.pixelSize: quranTheme.fontSizeLarge
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
                NumberAnimation { duration: 200 }
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
