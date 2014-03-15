// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

QuranPage {
        onStatusChanged: {
                if (status == PageStatus.Active) {
                        pageStack.pushAttached(Qt.resolvedUrl("PartSelectionPage.qml"))
                }
        }

        SilicaListView {
                id: view
                anchors {
                        fill: parent
                        leftMargin: 16
                        rightMargin: 16
                }

                model: _data.verseCount
                highlightFollowsCurrentItem: false
                currentIndex: _data.firstSuraForPage(_settings.pageNumber)
                Component.onCompleted: view.positionViewAtIndex(currentIndex, ListView.Center)

                delegate: ListDelegate {
                        number: index
                        text: _data.fullSuraName(index)
                        highlight: ListView.isCurrentItem
                        showPlay: _settings.recitationMode != 0 && _fsmon.available
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
