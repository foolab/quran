// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
        id: root
        property int sura
        width: parent.width
        height: Theme.itemSizeMedium
        property bool highlight: ListView.isCurrentItem

        Item {
                anchors.fill: parent

                NumberLabel {
                        id: number
                        color: root.highlight ? Theme.highlightColor : Theme.primaryColor
                        number: root.sura
                        width: 70
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                }

                Label {
                        width: parent.width - number.width
                        text: _data.fullSuraName(root.sura);
                        font.family: Theme.fontFamily
                        anchors.right: number.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        color: root.highlight ? Theme.highlightColor : Theme.primaryColor
                }
        }
}
