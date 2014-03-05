// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QuranPage {
        SilicaListView {
                id: view
                model: _data.partCount()
                anchors {
                        fill: parent
                        leftMargin: 16
                        rightMargin: 16
                }

// TODO: add back part recitation
                delegate: BackgroundItem {
                        width: view.width
                        height: Theme.itemSizeMedium

                        Label {
                                anchors.fill: parent
                                anchors.rightMargin: 20
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                text: _data.partName(index)
                        }

                        onClicked: {
                                _settings.pageNumber = _data.pageNumberForPart(index)
                                pageStack.pop()
                        }
                }

                header: PageHeader {
                        width: view.width
                        title: qsTr("Choose a part")
                }
        }
}
