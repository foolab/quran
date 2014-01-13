// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
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

        Component.onCompleted: {
            _bookmarks.bookmarkRemoved.connect(view.model.bookmarkRemoved);
        }

        Component.onDestruction: {
            _bookmarks.bookmarkRemoved.disconnect(view.model.bookmarkRemoved);
        }

        Component {
                id: sectionDelegate

                Label {
                        id: title
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        font.family: _settings.fontFamily
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
                                pagePosition.setPosition(sura, aya)
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
                                text: qsTr("(%1) %2").arg(_formatter.number(aya + 1)).arg(_data.text(sura, aya))
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignRight
                                color: Theme.primaryColor
                        }

                        RemorseItem { id: remorse }
                }
        }

        SilicaListView {
                id: view
                model: FavoritesModel {}
                anchors.fill: parent
                header: PageHeader {
                        width: parent.width
                        title: qsTr("Favorites")
                }

                section.property: "sura"
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
