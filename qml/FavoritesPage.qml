// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

QuranPage {
        id: favoritesPage
        property Item menu

        RemorsePopup {
                id: pageRemorse
        }

        Connections {
                target: _bookmarks
                onCleared: {
                        view.model.clear()
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
                id: contextMenuComponent
                ContextMenu {
                        id: menu

                        MenuItem {
                                text: qsTr("Remove")
                                onClicked: remorse.execute(item, "Removing", function() { _bookmarks.remove(bookmark) } )
                        }
                }
        }

        Component {
                id: favoritesPageDelegate

                BackgroundItem {
                        id: item
                        width: parent.width
                        height: menu && menu.parent == item ? label.height + menu.height : label.height

                        onPressAndHold: {
                                if (!menu) {
                                        menu = contextMenuComponent.createObject(view)
                                }

                                menu.show(item)
                        }

                        onClicked: {
                                pagePosition.setPosition(chapter, verse)
                                pageStack.pop()
                        }

                        QuranLabel {
                                id: label
                                anchors {
                                        top: parent.top
                                        right: parent.right
                                        left: parent.left
                                        rightMargin: theme.marginMedium
                                        leftMargin: theme.marginMedium
                                }

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

                        RemorseItem { id: remorse }
                }
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

                PullDownMenu {
                        MenuItem {
                                text: qsTr("Clear")
                                onClicked: {
                                        pageRemorse.execute("Clearing", function() { _bookmarks.clear() } )
                                }
                        }
                }
        }
}
