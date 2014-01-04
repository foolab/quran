// -*- qml -*-
import QtQuick 2.0
import Quran 1.0
import Sailfish.Silica 1.0

QuranPage {
        id: quranPage

        SilicaListView {
                id: view
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: toolBar.top
                model: _data.pageCount()
                orientation: ListView.Horizontal
                snapMode: ListView.SnapOneItem
                highlightRangeMode: ListView.StrictlyEnforceRange
                LayoutMirroring.enabled: true
                LayoutMirroring.childrenInherit: false
                interactive: false
                highlightFollowsCurrentItem: true
                highlightMoveVelocity: width * 2
                highlightMoveDuration: 0
                delegate: quranPageDelegate
                currentIndex: _settings.pageNumber
        }

        Component {
                id: quranPageDelegate

                QuranView {
                        id: content
                        width: view.width
                        height: view.height

                        PullDownMenu {
                                MenuItem { text: qsTr("About"); onClicked: { pageStack.push(Qt.resolvedUrl("AboutPage.qml")); } }
                                MenuItem { text: qsTr("Settings"); onClicked: { pageStack.push(Qt.resolvedUrl("SettingsPage.qml")); } }
//                                MenuItem { text: qsTr("Search"); onClicked: { pageStack.push(Qt.resolvedUrl("SearchPage.qml")); } }
                        }
                }
        }

/*
// TODO:
        Connections {
                target: _recitations
                onPositionChanged: {
                        pagePosition.setPosition(chapter, verse);
                }
        }

        TranslationSelector {
                id: translationSelector
        }

        RecitationSelector {
                id: recitationSelector
        }

*/
        ToolBar {
                id: toolBar
                ToolBarLayout {
                        id: layout
                        anchors.fill: parent
                        Component.onCompleted: setItems(children);

                        ToolButton {
                                image: theme.favorites
                                onClicked: pageStack.push(Qt.resolvedUrl("FavoritesPage.qml"))
                        }

                        ToolButton {
                                image: theme.translations
                                visible: _settings.translationMode != 0
                                onClicked: _fsmon.available ? translationSelector.open() : massStorage.show();
                        }

                        ToolButton {
                                image: theme.recitations
                                visible: _settings.recitationMode != 0
                                onClicked: _fsmon.available ? recitationSelector.open() : massStorage.show();
                        }

                        ToolButton {
                                image: theme.next
                                onClicked: {
                                        var newIndex = _settings.pageNumber + 1;
                                        if (_data.hasPage(newIndex)) {
                                                _settings.pageNumber = newIndex;
                                        }
                                        else {
                                                lastPageReached.show();
                                        }
                                }
                        }

                        NumberLabel {
                                width: 60
                                height: Theme.itemSizeSmall
                                number: _settings.pageNumber
                                onClicked: pageStack.push(Qt.resolvedUrl("IndexPage.qml"));
                        }

                        ToolButton {
                                image: theme.previous
                                onClicked: {
                                        var newIndex = _settings.pageNumber - 1;
                                        if (_data.hasPage(newIndex)) {
                                                _settings.pageNumber = newIndex;
                                        }
                                        else {
                                                firstPageReached.show();
                                        }
                                }
                        }

                }
        }
}
