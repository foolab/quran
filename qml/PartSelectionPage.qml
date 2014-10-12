// -*- qml -*-
import QtQuick 2.0

QuranPage {
        QuranPageMenu {
                view: view

                actions: [
                MenuAction {
                        text: qsTr("Choose chapter")
                        onClicked: replaceAnimated(Qt.resolvedUrl("IndexPage.qml"))
                }
                ]
        }

        QuranListView {
                id: view
                model: _data.partCount
                anchors.fill: parent
                highlightFollowsCurrentItem: false
                currentIndex: _data.partNumberForPage(settings.pageNumber)
                Component.onCompleted: view.positionViewAtIndex(currentIndex, ListView.Center)

                delegate: IndexDelegate {
                        number: index
                        text: _data.partName(index)
                        showPlay: settings.recitationMode != 0 && _recitations.installedCount > 0
                        showMore: false
                        highlight: ListView.isCurrentItem

                        onClicked: {
                            settings.pageNumber = _data.pageNumberForPart(index)
                            popPage()
                        }

                        onPlayClicked: {
                            settings.pageNumber = _data.pageNumberForPart(index)
                            _recitations.playPart(index)
                            popPage()
                        }
                }

                header: QuranPageHeader {
                        width: view.width
                        title: qsTr("Choose a part")
                }
        }
}
