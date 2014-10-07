// -*- qml -*-
import QtQuick 2.0
import Quran 1.0
import Sailfish.Silica 1.0

QuranPage {
        id: quranPage

        Rectangle {
                anchors.fill: parent
                color: theme.backgroundColor
        }

        SilicaListView {
                id: view
                anchors {
                        top: parent.top
                        bottom: row.top
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

        Row {
                id: row
                anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                }

                height: Theme.itemSizeSmall

                IconButton {
                        icon.source: "image://icon/reciter.png?"
                        onClicked: pageStack.push(Qt.resolvedUrl("RecitationSelector.qml"))
                        enabled: _settings.recitationMode != 0 && _recitations.installedCount > 0
                }

                IconButton {
                        icon.source: "image://icon/translation.png?"
                        onClicked: pageStack.push(Qt.resolvedUrl("TranslationSelector.qml"))
                        enabled: _settings.translationMode != 0 && _translations.installedCount > 0
                }

                NumberLabel {
                        width: 60
                        height: Theme.itemSizeSmall
                        number: _settings.pageNumber
                        onClicked: pageStack.push(Qt.resolvedUrl("IndexPage.qml"))
                        onPressAndHold: pageStack.push(Qt.resolvedUrl("PageSelectionPage.qml"))
                        color: highlight ? Theme.highlightColor : theme.textColor
                }

                IconButton {
                        icon.source: "image://icon/play.png?"
                        enabled: _settings.recitationMode != 0 && _recitations.installedCount > 0
                        onClicked: _recitations.playPage(_settings.pageNumber)
                }

                IconButton {
                        icon.source: "image://icon/stop.png?"
                        onClicked: _recitations.stop()
                        enabled: _recitations.isPlaying
                }

        }

}
