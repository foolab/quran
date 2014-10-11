// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        id: quranPage

        Rectangle {
                anchors.fill: parent
                color: theme.backgroundColor
        }

        QuranListView {
                id: view
                anchors.fill: parent
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
                currentIndex: settings.pageNumber
                onCurrentIndexChanged: {
                    if (currentIndex != -1) {
                        settings.pageNumber = currentIndex
                    }
                }
        }

        Component {
                id: quranPageDelegate

                QuranView {
                        QuranPageMenu {
                                view: view
                                actions: [
                                MenuAction { text: qsTr("About"); onClicked: { pushAnimated(Qt.resolvedUrl("AboutPage.qml")) } },
                                MenuAction { text: qsTr("Settings"); onClicked: { pushAnimated(Qt.resolvedUrl("SettingsPage.qml")) } },
                                MenuAction { text: qsTr("Search"); onClicked: { pushAnimated(Qt.resolvedUrl("SearchPage.qml")) } },
                                MenuAction { text: qsTr("Favorites"); onClicked: pushAnimated(Qt.resolvedUrl("FavoritesPage.qml")) }
                                ]
                        }
                }
        }

        Connections {
                target: _recitations
                onPositionChanged: {
                        pagePosition.setPosition(chapter, verse)
                }
        }

        toolBar: [
        ToolButton {
                width: theme.itemSizeSmall
                icon.source: highlight ? "image://icon/reciter.png?" + theme.buttonHighlightColor : "image://icon/reciter.png?" + theme.buttonNormalColor
                onClicked: pageStack.push(Qt.resolvedUrl("RecitationSelector.qml"))
                enabled: settings.recitationMode != 0 && _recitations.installedCount > 0
        },
        ToolButton {
                width: theme.itemSizeSmall
                icon.source: highlight ? "image://icon/translation.png?" + theme.buttonHighlightColor : "image://icon/translation.png?" + theme.buttonNormalColor
                onClicked: pageStack.push(Qt.resolvedUrl("TranslationSelector.qml"))
                enabled: settings.translationMode != 0 && _translations.installedCount > 0
        },
        NumberLabel {
                width: 60
                height: theme.itemSizeSmall
                number: settings.pageNumber + 1
                onClicked: pageStack.push(Qt.resolvedUrl("IndexPage.qml"))
                onPressAndHold: pageStack.push(Qt.resolvedUrl("PageSelectionPage.qml"))
                color: highlight ? theme.highlightColor : theme.textColor
        },
        ToolButton {
                width: theme.itemSizeSmall
                icon.source: highlight ? "image://icon/play.png?" + theme.buttonHighlightColor : "image://icon/play.png?" + theme.buttonNormalColor
                enabled: settings.recitationMode != 0 && _recitations.installedCount > 0
                onClicked: _recitations.playPage(settings.pageNumber)
        },
        ToolButton {
                width: theme.itemSizeSmall
                icon.source: highlight ? "image://icon/stop.png?" + theme.buttonHighlightColor : "image://icon/stop.png?" + theme.buttonNormalColor
                onClicked: _recitations.stop()
                enabled: _recitations.isPlaying
        }
        ]
}
