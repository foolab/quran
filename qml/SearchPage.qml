// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        id: searchPage
        menu: pageMenu

        property string searchString
        property Item searchField

        function doSearch() {
            searchModel.setQuery(searchPage.searchString, settings.searchMatchWholeWords)
        }

        Component {
                id: headerDelegate

                Column {
                        width: parent.width

                        QuranPageHeader {
                                width: parent.width
                                title: qsTr("Search")
                        }

                        QuranSearchField {
                                id: field
                                Component.onCompleted: searchPage.searchField = field
                                width: parent.width
                                onTextChanged: searchPage.searchString = text
                                enableSoftwareInputPanel: false
                                onEnterKeyClicked: {
                                    if (searchPage.searchString.length) {
                                        searchPage.doSearch()
                                    }
                                }
                        }

                        QuranTextSwitch {
                                width: parent.width
                                text: qsTr("Match whole words only");
                                checked: settings.searchMatchWholeWords
                                onCheckedChanged: settings.searchMatchWholeWords = checked
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
                                onClicked: deleter.confirm(item, qsTr("Removing"))
                        }
                        ]
                }
        }

        QuranPageMenu {
                id: pageMenu
                view: view
                actions: [
                MenuAction {
                        text: qsTr("Search")
                        onClicked: searchPage.doSearch()
                        visible: searchPage.searchString.length > 0
                }
                ]
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
            width: theme.itemSizeLarge
            height: width
            color: theme.keyboardPreviewBackgroundColor
            visible: text != ""
            radius: width / 3
            smooth: true
            border {
                    color: theme.keyboardPreviewTextColor
                    width: 1
            }
            anchors {
                bottom: keyboard.top
                horizontalCenter: parent.horizontalCenter
            }

            QuranLabel {
                color: theme.keyboardPreviewTextColor
                id: label
                anchors.fill: parent
                font.pixelSize: theme.fontSizeLarge
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

            opacity: searchPage.searchField.focus ? 1.0 : 0
            visible: opacity > 0

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
