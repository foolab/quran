// -*- qml -*-
import QtQuick 1.0

Item {
        id: numPad

        width: 180
        height: 240

        signal insert(int val);
        signal backspace

        Row {
                id: one
                anchors.top: parent.top
                width: parent.width
                NumPadButton {index: 1}
                NumPadButton {index: 2}
                NumPadButton {index: 3}
        }

        Row {
                id: two
                anchors.top: one.bottom
                width: parent.width
                NumPadButton {index: 4}
                NumPadButton {index: 5}
                NumPadButton {index: 6}
        }

        Row {
                id: three
                anchors.top: two.bottom
                width: parent.width
                NumPadButton {index: 7}
                NumPadButton {index: 8}
                NumPadButton {index: 9}
        }

        Row {
                id: four
                anchors.top: three.bottom
                width: parent.width
                NumPadButton {index: 0}
                Button {
                        width: parent.width * (2/3)
                        height: 60
                        text: "<<"
                        onClicked: numPad.backspace();
                }
        }
}
