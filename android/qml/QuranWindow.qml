// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

ApplicationWindow {
        property Item pageStack: stack

        property alias initialPage: stack.initialItem

        visible: true

        StackView {
                id: stack
                anchors.fill: parent
        }
}
