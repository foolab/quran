// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QuranPage {
        SilicaListView {
                id: view
                model: _data.partCount
                anchors {
                        fill: parent
                        leftMargin: theme.marginMedium
                        rightMargin: theme.marginMedium
                }

                highlightFollowsCurrentItem: false
                currentIndex: _data.partNumberForPage(settings.pageNumber)
                Component.onCompleted: view.positionViewAtIndex(currentIndex, ListView.Center)

                delegate: ListDelegate {
                        number: index
                        text: _data.partName(index)
                        showPlay: settings.recitationMode != 0 && _recitations.installedCount > 0
                        showMore: false
                        highlight: ListView.isCurrentItem

                        onClicked: {
                            settings.pageNumber = _data.pageNumberForPart(index)
                            pageStack.pop()
                        }

                        onPlayClicked: {
                            settings.pageNumber = _data.pageNumberForPart(index)
                            _recitations.playPart(index)
                            pageStack.pop()
                        }
                }

                header: PageHeader {
                        width: view.width
                        title: qsTr("Choose a part")
                }
        }
}
