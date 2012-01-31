// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: root
        signal clicked

        property int sura: 0
        property bool selected: false

        color: selected || mouse.pressed ? "steelblue" : colors.backgroundColor

        width: parent.width / 2
        height: verse.height * 2

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: root.clicked();
        }

        NumberLabel {
                color: colors.textColor
                id: number
                number: root.sura
                width: 70
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: parent.height/4
                horizontalAlignment: Text.AlignRight
                font.pointSize: 24
        }

        Label {
                id: verse
                font.pointSize: 24
                width: parent.width - number.width
                text: _data.fullSuraName(root.sura);
                font.bold: true
                anchors.right: number.left
                anchors.top: parent.top
                anchors.topMargin: parent.height/4
                horizontalAlignment: Text.AlignRight
                color: colors.textColor
        }
}
