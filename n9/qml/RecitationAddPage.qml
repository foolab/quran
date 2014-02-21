// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

Page {
	    id: recitationInstallPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Enable online recitations")
        }

        Label {
                anchors.centerIn: parent
                text: qsTr("Online recitations is disabled from settings.")
                width: parent.width
                font.pixelSize: 26
                horizontalAlignment: Text.AlignHCenter
                visible: !_settings.onlineRecitations
        }

        ListView {
                id: view
                visible: _settings.onlineRecitations
                clip: true
                anchors.top: title.bottom
                anchors.topMargin: 16
                anchors.bottom: toolBar.top
                anchors.bottomMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16

                model: InstallableRecitationsModel {
                        recitations: _recitations
                }

                delegate: recitationssDelegate
        }

        Component {
                id: recitationssDelegate

                Rectangle {
                        width: view.width
                        height: label.height * 2
                        color: mouse.pressed ? _colors.pressedColor : _colors.backgroundColor

                        Label {
                                id: label
                                text: qsTr("%1 (%2)").arg(name).arg(quality)
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                color: mouse.pressed ? _colors.pressedTextColor : _colors.textColor
                        }

                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                                onClicked: {
                                        if (!_recitations.enableInstallable(recitationId)) {
                                                banner.text = qsTr("Failed to enable recitation");
                                                banner.show();
                                        }
                                }
                        }
                }
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}
