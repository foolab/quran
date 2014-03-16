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

                model: _data.pageCount
                orientation: ListView.Horizontal
                snapMode: ListView.SnapOneItem
                highlightRangeMode: ListView.StrictlyEnforceRange
                LayoutMirroring.enabled: true
                LayoutMirroring.childrenInherit: false
                highlightFollowsCurrentItem: true
                highlightMoveVelocity: width * 2
                highlightMoveDuration: 0
                delegate: quranPageDelegate
                currentIndex: _settings.pageNumber
                onCurrentIndexChanged: {
                    if (currentIndex != -1) {
                        _settings.pageNumber = currentIndex
                    }
                }

                clip: true
        }

        Component {
                id: quranPageDelegate

                QuranView {
                        id: content
                        width: view.width
                        height: view.height

                        PullDownMenu {
                                MenuItem { text: qsTr("About"); onClicked: { pageStack.push(Qt.resolvedUrl("AboutPage.qml")) } }
                                MenuItem { text: qsTr("Settings"); onClicked: { pageStack.push(Qt.resolvedUrl("SettingsPage.qml")) } }
                                MenuItem { text: qsTr("Search"); onClicked: { pageStack.push(Qt.resolvedUrl("SearchPage.qml")) } }
                                MenuItem { text: qsTr("Favorites"); onClicked: pageStack.push(Qt.resolvedUrl("FavoritesPage.qml")) }
                        }
                }
        }

        Connections {
                target: _recitations
                onPositionChanged: {
                        pagePosition.setPosition(chapter, verse)
                }
        }

        SilicaFlickable {
                id: panel
                width: parent.width
                height: row.height
                contentHeight: height
                anchors.bottom: parent.bottom

                PushUpMenu {
                        visible: selectTranslation.enabled || selectRecitation.enabled

                        MenuItem {
                                id: selectTranslation
                                text: qsTr("Select translation")
                                enabled: _settings.translationMode != 0 && _translations.installedCount > 0
                                onClicked: pageStack.push(Qt.resolvedUrl("TranslationSelector.qml"))
                        }

                        MenuItem {
                                id: selectRecitation
                                text: qsTr("Select recitation")
                                enabled: _settings.recitationMode != 0 && _recitations.installedCount > 0
                                onClicked: pageStack.push(Qt.resolvedUrl("RecitationSelector.qml"))
                        }
                }

                Row {
                        id: row
                        anchors.centerIn: parent

                        IconButton {
                                icon.source: "image://theme/icon-m-play?black"
                                visible: _settings.recitationMode != 0 && _recitations.installedCount > 0
                                onClicked: _recitations.playPage(_settings.pageNumber)
                        }

                        NumberLabel {
                                width: 60
                                height: Theme.itemSizeSmall
                                number: _settings.pageNumber
                                onClicked: pageStack.push(Qt.resolvedUrl("IndexPage.qml"))
                                onPressAndHold: pageStack.push(Qt.resolvedUrl("PageSelectionPage.qml"))
                                color: _colors.textColor
                        }

                        IconButton {
                                icon.source: "image://icon/" + theme.stop
                                onClicked: _recitations.stop()
                                visible: _recitations.isPlaying
                        }
                }
        }
}
