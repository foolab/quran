// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

QuranPage {
	    id: recitationsListPage

        Component {
                id: recitationssDelegate
                // TODO: Context menu
                DownloadLabel {
                        showInstalled: isOnline
                        showProgress: false
                        text: name
                }
        }

        SilicaListView {
                id: view
                anchors.fill: parent
                model: RecitationModel {
                        recitations: _recitations
                }

                delegate: recitationssDelegate

                header: PageHeader {
                        width: parent.width
                        title: qsTr("Recitations")
                }

                ViewPlaceholder {
                        text: qsTr("No recitations. Pull down to enable a recitation.")
                        enabled: view.count == 0
                }
                PullDownMenu {
                        MenuItem {
                                text: qsTr("Add recitation")
                                onClicked: pageStack.push(Qt.resolvedUrl("RecitationAddPage.qml"))
                        }
                }
        }
}
