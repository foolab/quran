// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {

        QuranPageMenu {
                view: view

                actions: [
                MenuAction {
                        text: qsTr("Choose part")
                        onClicked: replaceAnimated(Qt.resolvedUrl("PartSelectionPage.qml"))
                }
                ]
        }

        QuranListView {
                id: view
                anchors.fill: parent
                model: _data.verseCount
                highlightFollowsCurrentItem: false
                currentIndex: _data.firstSuraForPage(settings.pageNumber)
                Component.onCompleted: view.positionViewAtIndex(currentIndex, ListView.Center)

                delegate: IndexDelegate {
                        number: index
                        text: _data.fullSuraName(index)
                        highlight: ListView.isCurrentItem
                        showPlay: settings.recitationMode != 0 && _recitations.installedCount > 0
                        onClicked: {
                                pagePosition.setPosition(index, 0)
                                pageStack.pop()
                        }

                        onPlayClicked: {
                                pagePosition.setPosition(index, 0)
                                _recitations.playChapter(index)
                                pageStack.pop()
                        }

                        onMoreClicked: {
                                pushAnimated(Qt.resolvedUrl("VerseSelectionPage.qml"), {chapter: number})
                        }
                }

                header: QuranPageHeader {
                        width: view.width
                        title: qsTr("Index")
                }
        }
}
