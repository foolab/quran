// -*- qml-mode -*-
import QtQuick 1.0

Item {
        id: window

        property alias pageStack: stack

        // TODO: detect the orientation and set the initial value accordingle.
        width: 424
        height: 800

        PageStack {
                id: stack
//                anchors.fill: parent
//                parent: window
        }
}
