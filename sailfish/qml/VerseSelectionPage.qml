// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QuranPage {
        id: page
        property int chapter

        SilicaGridView {
                id: grid
                model: _data.suraSize(parent.chapter)
                cellWidth: grid.width / 7
                cellHeight: cellWidth

                anchors {
                        fill: parent
                        leftMargin: theme.marginMedium
                        rightMargin: theme.marginMedium
                }

                header: PageHeader {
                        width: grid.width
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
                                number: index + 1
                                enableSignals: false
                        }

                        onClicked: {
                                pagePosition.setPosition(page.chapter, index)
                                pageStack.pop(pageStack.previousPage(pageStack.previousPage()))
                        }
                }
        }
}
