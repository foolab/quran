// -*- qml-mode -*-
import QtQuick 1.0

// TODO: use positionViewAtIndex ?
Page {
        id: indexPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Index")
        }

        Component {
                id: indexPageDelegate
                Item {
                        width: view.width
                        height: Math.max(left.height, right.height);
                        IndexPageCell { id: left; sura: index + 57 }
                        IndexPageCell { id: right; anchors.left: left.right; sura: index }
                }
        }

        ListView {
                id: view
                clip: true
                model: _data.suraCount()/2;
                anchors.top: title.bottom
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.bottom: toolBar.top
                delegate: indexPageDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
                }
        }
}
