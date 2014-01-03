// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QuranPage {
        id: favoritesPage

        RemorsePopup {
                id: pageRemorse
        }

        Label {
                id: error
                text: qsTr("No favorites added.\nTap on a verse then tap the star button to add or remove favorites")
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.top: parent.top
                anchors.topMargin: 26
                width: parent.width
                font.pixelSize: 26
                horizontalAlignment: Text.AlignHCenter
                color: _colors.textColor
                visible: _bookmarks.empty
                wrapMode: Text.WordWrap
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
                Rectangle {
                        width: view.width
                        color: _colors.sectionColor
                        height: title.height

                        Label {
                                anchors.right: parent.right
                                anchors.rightMargin: 20
                                id: title
                                font.pointSize: 26
                                font.family: _settings.fontFamily
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                text: _data.fullSuraName(section)
                        }
                }
        }

        Component {
                id: favoritesPageDelegate

                BackgroundItem {
                        id: item
                        width: parent.width
                        height: menu.active ? Theme.itemSizeSmall + menu.height : Theme.itemSizeSmall

                        onClicked: {
                                pagePosition.setPosition(sura, aya)
                                pageStack.pop()
                        }

                        Label {
                                anchors.fill: parent
                                font.pointSize: 18
                                font.family: _settings.fontFamily
                                text: qsTr("(%1) %2").arg(_formatter.number(aya + 1)).arg(_data.text(sura, aya))
                                elide: Text.ElideRight
                                horizontalAlignment: Text.AlignRight
                                color: _colors.textColor
                        }

                        onPressAndHold: menu.show(item)

                        ContextMenu {
                                id: menu
                                MenuItem {
                                        text: qsTr("Remove")
                                        onClicked: remorse.execute(item, "Removing", function() { _bookmarks.remove(bookmark) } )

                                }
                        }

                        RemorseItem { id: remorse }
                }
        }

        SilicaListView {
                id: view
                clip: true
                model: FavoritesModel {}
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.bottom: parent.bottom
                header: PageHeader {
                        width: parent.width
                        title: qsTr("Favorites")
                }

                section.property: "sura"
                section.criteria: ViewSection.FullString
                section.delegate: sectionDelegate
                delegate: favoritesPageDelegate

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
