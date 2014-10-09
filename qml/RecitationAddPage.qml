// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
	    id: recitationInstallPage

        Component {
                id: recitationssDelegate

                DownloadLabel {
                        showProgress: false
                        showInstalled: installed
                        text: qsTr("%1 (%2)").arg(name).arg(quality)
                        onClicked: toggleRecitation(recitationId, installed);
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
