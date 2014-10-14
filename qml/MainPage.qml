// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        id: quranPage
        menu: view.currentItem ? view.currentItem.menu : null

        Rectangle {
                anchors.fill: parent
                color: quranTheme.backgroundColor
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
                        property alias menu: pageMenu
                        QuranPageMenu {
                                id: pageMenu
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
                icon.source: highlight ? "image://icon/reciter.png?" + quranTheme.buttonHighlightColor : "image://icon/reciter.png?" + quranTheme.buttonNormalColor
                onClicked: pushAnimated(Qt.resolvedUrl("RecitationSelector.qml"))
                enabled: settings.recitationMode != 0 && _recitations.installedCount > 0
        },
        ToolButton {
                icon.source: highlight ? "image://icon/translation.png?" + quranTheme.buttonHighlightColor : "image://icon/translation.png?" + quranTheme.buttonNormalColor
                onClicked: pushAnimated(Qt.resolvedUrl("TranslationSelector.qml"))
                enabled: settings.translationMode != 0 && _translations.installedCount > 0
        },
        NumberLabel {
                width: 60
                height: quranTheme.itemSizeSmall
                number: settings.pageNumber + 1
                onClicked: pushAnimated(Qt.resolvedUrl("IndexPage.qml"))
                onPressAndHold: pushAnimated(Qt.resolvedUrl("PageSelectionPage.qml"))
                color: highlight ? quranTheme.highlightColor : quranTheme.textColor
        },
        ToolButton {
                icon.source: highlight ? "image://icon/play.png?" + quranTheme.buttonHighlightColor : "image://icon/play.png?" + quranTheme.buttonNormalColor
                enabled: settings.recitationMode != 0 && _recitations.installedCount > 0
                onClicked: _recitations.playPage(settings.pageNumber)
        },
        ToolButton {
                icon.source: highlight ? "image://icon/stop.png?" + quranTheme.buttonHighlightColor : "image://icon/stop.png?" + quranTheme.buttonNormalColor
                onClicked: _recitations.stop()
                enabled: _recitations.isPlaying
        }
        ]
}
