// -*- qml-mode -*-
import QtQuick 1.0

// TODO: use positionViewAtIndex ?
Page {
        id: indexPage

        tools: toolBar

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
                anchors.top: parent.top
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.bottom: toolBar.top
                anchors.bottomMargin: 16
                delegate: indexPageDelegate
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
                }
        }
}
