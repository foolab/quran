// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
	    id: recitationInstallPage

        Component {
                id: recitationssDelegate

                ContextMenuLabel {
                        onClicked: toggleRecitation(recitationId, installed)
                        content: [
                        QuranStatusIndicator {
                                id: indicator
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                isInstalled: installed
                        },
                        QuranLabel {
                                text: qsTr("%1 (%2)").arg(name).arg(quality)
                                anchors {
                                        top: parent.top
                                        bottom: parent.bottom
                                        right: indicator.left
                                        rightMargin: theme.marginSmall
                                        left: parent.left
                                        leftMargin: theme.marginSmall
                                }

                                verticalAlignment: Text.AlignVCenter
                        }
                        ]
                }
        }

        function toggleRecitation(rid, installed) {
                if (installed) {
                        _recitations.disableInstallable(rid)
                }
                else {
                        _recitations.enableInstallable(rid)
                }
        }

        QuranListView {
                anchors.fill: parent

                model: InstallableRecitationsModel {
                        recitations: _recitations
                }

                delegate: recitationssDelegate

                header: QuranPageHeader {
                        width: parent.width
                        title: qsTr("Enable online recitations")
                }

        }
}
