// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
        SilicaGridView {
                id: view
                model: _data.pageCount()
                anchors {
                        fill: parent
                        leftMargin: 16
                        rightMargin: 16
                }

                Component.onCompleted: positionViewAtIndex(_settings.pageNumber, GridView.Center)
                cellWidth: view.width / 7
                cellHeight: cellWidth

                delegate: BackgroundItem {
                        width: view.cellWidth
                        height: view.cellHeight
                        NumberLabel {
                                anchors.fill: parent
                                color: index == _settings.pageNumber ? Theme.highlightColor : Theme.primaryColor
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                number: index
                                enableSignals: false
                        }

                        onClicked: {
                                _settings.pageNumber = index
                                pageStack.pop()
                        }
                }

                header: PageHeader {
                        width: view.width
                        title: qsTr("Choose a page")
                }
        }
}
