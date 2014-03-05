// -*- qml -*-
import QtQuick 2.0
import Quran 1.0
import Sailfish.Silica 1.0

QuranPage {
        id: quranPage

        Rectangle {
                anchors.fill: parent
                color: _colors.backgroundColor
        }

        SilicaListView {
                id: view
                anchors {
                        top: parent.top
                        bottom: panel.top
                        right: parent.right
                        left: parent.left
                }

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
                clip: true
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
                                MenuItem { text: qsTr("Favorites"); onClicked: pageStack.push(Qt.resolvedUrl("FavoritesPage.qml")) }
                        }
                }
        }

        Connections {
                target: _recitations
                onPositionChanged: {
                        pagePosition.setPosition(chapter, verse);
                }
        }

/*
        RecitationSelector {
                id: recitationSelector
        }

*/

        SilicaFlickable {
                id: panel
                width: parent.width
                height: row.height
                contentHeight: height
                anchors.bottom: parent.bottom

                PushUpMenu {
                        // TODO: not working?
//                        visible: selectTranslation.visible || selectRecitation.visible

                        MenuItem {
                                id: selectTranslation
                                text: qsTr("Select translation")
                                visible: _settings.translationMode != 0
                                onClicked: pageStack.push(Qt.resolvedUrl("TranslationSelector.qml"))
                        }

                        MenuItem {
                                id: selectRecitation
                                text: qsTr("Select recitation")
                                visible: _settings.recitationMode != 0 // && _recitations.installed > 1
                                onClicked: pageStack.push(Qt.resolvedUrl("RecitationSelector.qml"))
                        }
                }

                Row {
                        id: row
                        anchors.centerIn: parent

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
                                onPressAndHold: pageStack.push(Qt.resolvedUrl("PageSelectionPage.qml"));
                                color: _colors.textColor
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
