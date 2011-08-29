// -*- qml-mode -*-
import QtQuick 1.0

Page {
        id: favoritesPageEmpty

        tools: toolBar

        Label {
                id: title
                text: qsTr("Favorites")
                font.pixelSize: 32
                anchors.top: parent.top
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 16
        }

        Label {
                id: error
                text: qsTr("No favorites. Long tap on a verse to add or remove favorites")
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.top: title.bottom
                anchors.topMargin: 26
                width: parent.width
                font.pixelSize: 26
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
                }
        }

}
