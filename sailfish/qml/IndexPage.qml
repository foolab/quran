// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

QuranPage {
        id: indexPage

        onStatusChanged: {
                if (status == PageStatus.Active) {
                        pageStack.pushAttached(Qt.resolvedUrl("PartSelectionPage.qml"))
                }
        }

        Drawer {
                id: drawer
                anchors.fill: parent
                hideOnMinimize: true

                background: SilicaGridView {
                        id: grid
                        property int chapter: -1
                        model: grid.chapter == -1 ? undefined : _data.suraSize(grid.chapter)
                        cellWidth: view.width / 7
                        cellHeight: cellWidth
                        anchors {
                                fill: parent
                                leftMargin: 16
                                rightMargin: 16
                        }

                        header: PageHeader {
                                width: view.width
                                title: qsTr("Choose a verse")
                        }

                        delegate: BackgroundItem {
                                width: grid.cellWidth
                                height: grid.cellHeight
                                NumberLabel {
                                        anchors.fill: parent
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        number: index
                                        enableSignals: false
                                }

                                onClicked: {
                                        pagePosition.setPosition(grid.chapter, index)
                                        pageStack.pop()
                                }
                        }
                }

                // TODO: not dimmed when drawer is open?
                foreground: SilicaListView {
                        id: view
                        model: _data.verseCount
                        highlightFollowsCurrentItem: false
                        currentIndex: _data.firstSuraForPage(_settings.pageNumber)
                        Component.onCompleted: view.positionViewAtIndex(currentIndex, ListView.Center)
                        anchors {
                                fill: parent
                                leftMargin: 16
                                rightMargin: 16
                        }

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

                            onPressAndHold: {
                                grid.chapter = index
                                drawer.open = true
                            }
                        }

                        header: PageHeader {
                                width: view.width
                                title: qsTr("Index")
                        }
                }
        }
}
