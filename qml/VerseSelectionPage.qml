// -*- qml -*-
import QtQuick 2.0

QuranPage {
        id: page
        property int chapter

        QuranGridView {
                id: grid
                model: _data.suraSize(page.chapter)
                cellWidth: grid.width / 7
                cellHeight: cellWidth

                anchors {
                        fill: parent
                        leftMargin: quranTheme.marginMedium
                        rightMargin: quranTheme.marginMedium
                }

                header: QuranPageHeader {
                        width: grid.width
                        title: qsTr("Choose a verse")
                }

                delegate: QuranBackgroundItem {
                        width: grid.cellWidth
                        height: grid.cellHeight
                        NumberLabel {
                                anchors.fill: parent
                                color: quranTheme.primaryColor
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                number: index + 1
                                enableSignals: false
                        }

                        onClicked: {
                                pagePosition.setPosition(page.chapter, index)
                                popToFirst()
                        }
                }
        }
}
