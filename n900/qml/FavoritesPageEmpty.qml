// -*- qml-mode -*-
import QtQuick 1.0

Page {
        id: favoritesPageEmpty

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Favorites")
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
