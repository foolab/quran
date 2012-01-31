// -*- qml -*-
import QtQuick 1.0

Page {
	    id: recitationsListPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Recitations")
        }

        Component {
                id: recitationssDelegate

                Rectangle {
                        width: view.width
                        height: label.height * 2
                        color: mouse.pressed ? "steelblue" : colors.backgroundColor

                        Label {
                                id: label
                                text: _recitations.recitationName(modelData);
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                color: mouse.pressed ? "black" : colors.textColor
                        }

                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                        }
                }
        }

        ListView {
                id: view
                clip: true
                anchors.top: title.bottom
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                model: _recitations.installed
                delegate: recitationssDelegate
        }

        // TODO: Refresh button.
        // TODO: hide list in mass storage mode ?
        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
                        ToolButton { icon: theme.addRecitation; onClicked: pageStack.push("RecitationAddPage"); }
		        }
		}
}
