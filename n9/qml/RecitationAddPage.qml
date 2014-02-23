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

        ListView {
                id: view
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

        function toggleRecitation(rid, installed) {
                if (installed) {
                        if (!_recitations.disableInstallable(rid)) {
                                banner.text = qsTr("Failed to disable recitation");
                                banner.show();
                        }
                }
                else {
                        if (!_recitations.enableInstallable(rid)) {
                                banner.text = qsTr("Failed to enable recitation");
                                banner.show();
                        }
                }
        }

        Component {
                id: recitationssDelegate

                DownloadLabel {
                        showProgress: false
                        showInstalled: installed
                        text: qsTr("%1 (%2)").arg(name).arg(quality)
                        onClicked: toggleRecitation(recitationId, installed);
                }
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
		        }
		}
}
