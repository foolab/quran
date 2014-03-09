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

                Label {
                        id: title
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        font.family: Theme.fontFamily
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: _data.fullSuraName(section)
                        color: Theme.highlightColor
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

                        Label {
                                id: label
                                anchors {
                                        top: parent.top
                                        right: parent.right
                                        left: parent.left
                                        rightMargin: 16
                                        leftMargin: 16
                                }

                                font.family: _settings.fontFamily
                                text: qsTr("(%1) %2").arg(_formatter.number(verse + 1)).arg(_data.text(chapter, verse))
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignRight
                                color: Theme.primaryColor
                        }

                        RemorseItem { id: remorse }
                }
        }

        SilicaListView {
                id: view
                anchors.fill: parent

                model: BookmarksModel {
                        bookmarks: _bookmarks
                }

                header: PageHeader {
                        width: parent.width
                        title: qsTr("Favorites")
                }

                section.property: "chapter"
                section.criteria: ViewSection.FullString
                section.delegate: sectionDelegate
                delegate: favoritesPageDelegate

                ViewPlaceholder {
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
