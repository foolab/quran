// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

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
                        color: mouse.pressed ? _colors.pressedColor : _colors.backgroundColor

                        Label {
                                id: label
                                text: name
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                color: mouse.pressed ? _colors.pressedTextColor : _colors.textColor
                        }

                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                                onClicked: {
                                        if (!isOnline) {
                                                return;
                                        }

                                        if (!_recitations.disableInstallable(recitationId)) {
                                                banner.text = qsTr("Failed to disable recitation");
                                                banner.show();
                                        } else {
                                                banner.text = qsTr("Recitation disabled");
                                                banner.show();
                                        }
                                }
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
                model: RecitationModel {
                        recitations: _recitations
                }

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
