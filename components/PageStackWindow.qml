// -*- qml-mode -*-
import QtQuick 1.0

Item {
        id: window

        property alias pageStack: stack

        // This has to be set to something in order for SizeRootObjectToView to work
        width: 1
        height: 1

        PageStack {
                id: stack
        }
}
