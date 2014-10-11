// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
        property alias title: header.acceptText
        property alias content: mainContent.children

        DialogHeader {
                id: header
                width: parent.width
        }

        Column {
                id: mainContent
                anchors {
                        top: header.bottom
                        bottom: parent.bottom
                        left: parent.left
                        right: parent.right
                }
        }
}
