// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

QuranPage {

        SilicaListView {
                id: view
                anchors {
                        fill: parent
                        leftMargin: theme.marginMedium
                        rightMargin: theme.marginMedium
                }

                PullDownMenu {
                        MenuItem { text: qsTr("Choose part"); onClicked: { pageStack.replace(Qt.resolvedUrl("PartSelectionPage.qml"), PageStackAction.Animated) } }
                }

                model: _data.verseCount
                highlightFollowsCurrentItem: false
                currentIndex: _data.firstSuraForPage(settings.pageNumber)
                Component.onCompleted: view.positionViewAtIndex(currentIndex, ListView.Center)

                delegate: ListDelegate {
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
                                pageStack.push(Qt.resolvedUrl("VerseSelectionPage.qml"), {chapter: index})
                        }
                }

                header: PageHeader {
                        width: view.width
                        title: qsTr("Index")
                }
        }
}
