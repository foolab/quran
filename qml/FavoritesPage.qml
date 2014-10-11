// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        id: favoritesPage

        DeletePopup {
                id: popup
                onConfirmed: _bookmarks.clear()
        }

        Connections {
                target: _bookmarks
                onCleared: {
                        pageStack.pop()
                }
        }

        Component {
                id: sectionDelegate

                QuranLabel {
                        id: title
                        anchors.right: parent.right
                        anchors.rightMargin: theme.largeMedium
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: _data.fullSuraName(section)
                        color: theme.highlightColor
                }
        }

        Component {
                id: favoritesPageDelegate

                ContextMenuLabel {
                        id: item
                        contentHeight: label.height

                        actions: [
                        MenuAction {
                                text: qsTr("Remove")
                                onClicked: deleter.confirm(item, qsTr("Removing"))
                        }
                        ]

                        onClicked: {
                                pagePosition.setPosition(chapter, verse)
                                pageStack.pop()
                        }

                        DeleteItem {
                                id: deleter
                                onConfirmed: _bookmarks.remove(bookmark)
                        }

                        content: QuranLabel {
                                id: label
                                width: parent.width

                                NumberFormatter {
                                        id: formatter
                                        format: settings.numberFormat
                                        number: verse + 1
                                }

                                font.family: settings.fontFamily
                                text: qsTr("(%1) %2").arg(formatter.formattedNumber).arg(_data.text(chapter, verse))
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignRight
                                color: theme.primaryColor
                        }
                }
        }

        QuranPageMenu {
                view: view
                actions: [
                MenuAction {
                        text: qsTr("Clear")
                        onClicked: popup.confirm(qsTr("Clearing"))
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
                        text: qsTr("No favorites added.\nTap on a verse then tap the star button to add or remove favorites")
                        enabled: _bookmarks.empty
                }
        }
}
